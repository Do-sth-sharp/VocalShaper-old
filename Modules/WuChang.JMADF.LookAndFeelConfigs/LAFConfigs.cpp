#include "LAFConfigs.h"
#include <libJModule.h>

bool LAFConfigs::getNum(const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, double& value)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	juce::var result;
	if (!this->getDFS(caller, caller, className, widget, attribute, "number", result)) {
		return false;
	}
	
	if (!(result.isDouble() || result.isInt() || result.isInt64())) {
		return false;
	}
	
	value = result;
	return true;
}

bool LAFConfigs::getString(const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, juce::String& value)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	juce::var result;
	if (!this->getDFS(caller, caller, className, widget, attribute, "string", result)) {
		return false;
	}

	if (!result.isString()) {
		return false;
	}

	value = result.toString();
	return true;
}

bool LAFConfigs::getBool(const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, bool& value)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	juce::var result;
	if (!this->getDFS(caller, caller, className, widget, attribute, "boolean", result)) {
		return false;
	}

	if (!result.isBool()) {
		return false;
	}

	value = result;
	return true;
}

void LAFConfigs::releaseAll()
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	this->list.clear();
}

void LAFConfigs::release(const juce::String& moduleName)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	for (auto it = this->list.begin(); it != this->list.end();) {
		if (it->first.first == moduleName) {
			it = this->list.erase(it);
			continue;
		}
		it++;
	}
}

void LAFConfigs::close(const juce::String& caller)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	for (auto it = this->list.begin(); it != this->list.end();) {
		it->second.modules.erase(caller);
		if (it->second.modules.empty()) {
			it = this->list.erase(it);
			continue;
		}
		it++;
	}
}

bool LAFConfigs::getDFS(const juce::String& caller, const juce::String& moduleName, const juce::String& className, const juce::String& widget, const juce::String& attribute, const juce::String& type, juce::var& value)
{
	juce::var* configTable = nullptr;
	if (!this->load(caller, moduleName, className, configTable)) {
		return false;
	}//获取配置表
	
	juce::var widgetTable = configTable->getProperty(widget, juce::var());//查找widget
	if (widgetTable.isObject()) {
		juce::var result = widgetTable.getProperty(attribute + ":" + type, juce::var());//强类型优先
		if (result.isVoid()) {
			result = widgetTable.getProperty(attribute, juce::var());
		}//弱类型次之
		if (!result.isVoid()) {
			value = result;
			return true;
		}//找到为止
	}
	
	widgetTable = configTable->getProperty("parent", juce::var());//查找父表
	if (widgetTable.isObject()) {
		juce::var classVar = widgetTable.getProperty("class", juce::var());
		juce::var moduleVar = widgetTable.getProperty("module", juce::var());
		
		if (classVar.isString()) {
			if (moduleVar.isVoid()) {
				return this->getDFS(caller, moduleName, classVar.toString(), widget, attribute, type, value);
			}//同模块查找
			else if (moduleVar.isString()) {
				return this->getDFS(caller, moduleVar.toString(), classVar.toString(), widget, attribute, type, value);
			}//跨模块查找
		}//有效的父表
	}
	
	return false;//无父表未找到值则返回false
}

bool LAFConfigs::load(const juce::String& caller, const juce::String& moduleName, const juce::String& className, juce::var*& data)
{
	LAFFile& laf = this->list[std::make_pair(moduleName, className)];
	//如果配置表存在则获取，不存在则建立
	
	laf.modules.insert(caller);//调用模块名入集合
	
	if (laf.flag) {
		data = &(laf.data);
		return true;
	}//配置表存在
	
	const jmadf::ModuleInfo* info = jmadf::FindModule(moduleName);
	if (!info) {
		this->list.erase(std::make_pair(moduleName, className));
		data = nullptr;
		return false;
	}//模块不存在

	juce::String filePath = info->path + "/.laf/" + className + ".json";//配置表文件位置
	juce::File configFile(filePath);
	if (!configFile.existsAsFile()) {
		this->list.erase(std::make_pair(moduleName, className));
		data = nullptr;
		return false;
	}//文件不存在
	
	laf.data = juce::JSON::parse(configFile);
	if (!laf.data.isObject()) {
		this->list.erase(std::make_pair(moduleName, className));
		data = nullptr;
		return false;
	}//文件格式不正确
	
	laf.flag = true;
	data = &(laf.data);
	return true;
}