#include "LAFConfigs.h"
#include <libJModule.h>

bool LAFConfigs::getNum(const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, double& value)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	juce::var result;
	std::set<std::pair<juce::String, juce::String>> inheritList;
	if (!this->getDFS(caller, caller, className, group, attribute,
		VarType::Number, result, inheritList)) {
		return false;
	}
	
	value = result;
	return true;
}

bool LAFConfigs::getString(const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, juce::String& value)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	juce::var result;
	std::set<std::pair<juce::String, juce::String>> inheritList;
	if (!this->getDFS(caller, caller, className, group, attribute,
		VarType::String, result, inheritList)) {
		return false;
	}

	value = result.toString();
	return true;
}

bool LAFConfigs::getBool(const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, bool& value)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	juce::var result;
	std::set<std::pair<juce::String, juce::String>> inheritList;
	if (!this->getDFS(caller, caller, className, group, attribute,
		VarType::Boolean, result, inheritList)) {
		return false;
	}

	value = result;
	return true;
}

bool LAFConfigs::getColour(const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, juce::Colour& value)
{
	juce::String cString;
	if (!this->getString(caller, className, group, attribute, cString)) {
		return false;
	}
	
	if (cString.startsWithChar('#')) {
		if (!cString.getLastCharacters(cString.length() - 1).containsOnly("0123456789abcdefABCDEF")) {
			return false;
		}
		if (cString.length() == 7) {
			uint8_t r = this->HexToDec(cString[1]) * 16 + this->HexToDec(cString[2]);
			uint8_t g = this->HexToDec(cString[3]) * 16 + this->HexToDec(cString[4]);
			uint8_t b = this->HexToDec(cString[5]) * 16 + this->HexToDec(cString[6]);
			value = juce::Colour(r, g, b);
			return true;
		}
		else if (cString.length() == 9) {
			uint8_t r = this->HexToDec(cString[1]) * 16 + this->HexToDec(cString[2]);
			uint8_t g = this->HexToDec(cString[3]) * 16 + this->HexToDec(cString[4]);
			uint8_t b = this->HexToDec(cString[5]) * 16 + this->HexToDec(cString[6]);
			uint8_t a = this->HexToDec(cString[7]) * 16 + this->HexToDec(cString[8]);
			value = juce::Colour(r, g, b, a);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (!cString.containsOnly("0123456789abcdefABCDEF")) {
			return false;
		}
		if (cString.length() == 6) {
			uint8_t r = this->HexToDec(cString[0]) * 16 + this->HexToDec(cString[1]);
			uint8_t g = this->HexToDec(cString[2]) * 16 + this->HexToDec(cString[3]);
			uint8_t b = this->HexToDec(cString[4]) * 16 + this->HexToDec(cString[5]);
			value = juce::Colour(r, g, b);
			return true;
		}
		else if (cString.length() == 8) {
			uint8_t r = this->HexToDec(cString[0]) * 16 + this->HexToDec(cString[1]);
			uint8_t g = this->HexToDec(cString[2]) * 16 + this->HexToDec(cString[3]);
			uint8_t b = this->HexToDec(cString[4]) * 16 + this->HexToDec(cString[5]);
			uint8_t a = this->HexToDec(cString[6]) * 16 + this->HexToDec(cString[7]);
			value = juce::Colour(r, g, b, a);
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

void LAFConfigs::releaseAll()
{
	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.clear();

	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	this->list.clear();
}

void LAFConfigs::release(const juce::String& moduleName)
{
	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.erase(moduleName);

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
	juce::ScopedWriteLock sLock(this->setLock);
	if (this->mSet.find(caller) == this->mSet.end()) {
		return;
	}
	this->mSet.erase(caller);

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

bool LAFConfigs::getDFS(const juce::String& caller, const juce::String& moduleName, const juce::String& className, const juce::String& group, const juce::String& attribute, VarType type, juce::var& value,
	std::set<std::pair<juce::String, juce::String>>& inheritList)
{
	if (inheritList.contains(std::make_pair(moduleName, className))) {
		return false;
	}
	juce::var* configTable = nullptr;
	if (!this->load(caller, moduleName, className, configTable)) {
		return false;
	}//获取配置表

	juce::String typeStr;
	switch (type)
	{
	case LAFConfigs::VarType::Number:
	{
		typeStr = "number";
		break;
	}
	case LAFConfigs::VarType::Boolean:
	{
		typeStr = "boolean";
		break;
	}
	case LAFConfigs::VarType::String:
	{
		typeStr = "string";
		break;
	}
	default:
		break;
	}
	
	juce::var groupTable = configTable->getProperty(group, juce::var());//查找group
	if (groupTable.isObject()) {
		juce::var result = groupTable.getProperty(attribute + ":" + typeStr, juce::var());//强类型优先
		if (result.isVoid()) {
			result = groupTable.getProperty(attribute, juce::var());
		}//弱类型次之
		if (!result.isVoid()) {
			bool varResult = true;
			if (result.isString() && result.toString().startsWithChar('$')) {
				std::set<juce::String> refList;
				varResult = this->getVar(caller, moduleName, className,
					result.toString().substring(1),
					type, result, inheritList, refList);
			}//如果为变量
			if (!varResult) {
				if (type != VarType::String) {
					return false;
				}
			}//如果未找到对应变量
			if (type == VarType::Number && !(result.isDouble() || result.isInt() || result.isInt64())) {
				return false;
			}
			else if (type == VarType::Boolean && !result.isBool()) {
				return false;
			}
			else if (type == VarType::String && !result.isString()) {
				return false;
			}//如果变量类型不匹配
			value = result;
			return true;
		}//找到为止
	}
	
	groupTable = configTable->getProperty("parent", juce::var());//查找父表
	if (groupTable.isObject()) {
		juce::var classVar = groupTable.getProperty("class", juce::var());
		juce::var moduleVar = groupTable.getProperty("module", juce::var());
		
		if (classVar.isString()) {
			if (moduleVar.isVoid()) {
				inheritList.insert(std::make_pair(moduleName, className));
				return this->getDFS(caller, moduleName, classVar.toString(), group, attribute,
					type, value, inheritList);
			}//同模块查找
			else if (moduleVar.isString()) {
				inheritList.insert(std::make_pair(moduleName, className));
				return this->getDFS(caller, moduleVar.toString(), classVar.toString(), group, attribute,
					type, value, inheritList);
			}//跨模块查找
		}//有效的父表
	}
	
	return false;//无父表未找到值则返回false
}

bool LAFConfigs::getVar(const juce::String& caller, const juce::String& moduleName, const juce::String& className, const juce::String& variable, VarType type, juce::var& value,
	std::set<std::pair<juce::String, juce::String>>& inheritList, std::set<juce::String>& refList)
{
	if (refList.contains(variable)) {
		return false;
	}
	if (inheritList.contains(std::make_pair(moduleName, className))) {
		return false;
	}

	juce::var* configTable = nullptr;
	if (!this->load(caller, moduleName, className, configTable)) {
		return false;
	}//获取配置表

	juce::var result = configTable->getProperty(variable, juce::var());//查找变量
	if (!result.isVoid()) {
		bool varResult = true;
		if (result.isString() && result.toString().startsWithChar('$')) {
			refList.insert(variable);
			varResult = this->getVar(caller, moduleName, className,
				result.toString().substring(1),
				type, result, inheritList, refList);
		}//如果为变量
		if (!varResult) {
			if (type != VarType::String) {
				return false;
			}
		}//如果未找到对应变量
		if (type == VarType::Number && !(result.isDouble() || result.isInt() || result.isInt64())) {
			return false;
		}
		else if (type == VarType::Boolean && !result.isBool()) {
			return false;
		}
		else if (type == VarType::String && !result.isString()) {
			return false;
		}//如果变量类型不匹配
		value = result;
		return true;
	}//找到为止

	juce::var groupTable = configTable->getProperty("parent", juce::var());//查找父表
	if (groupTable.isObject()) {
		juce::var classVar = groupTable.getProperty("class", juce::var());
		juce::var moduleVar = groupTable.getProperty("module", juce::var());

		if (classVar.isString()) {
			if (moduleVar.isVoid()) {
				refList.clear();
				inheritList.insert(std::make_pair(moduleName, className));
				return this->getVar(caller, moduleName, classVar.toString(), variable,
					type, value, inheritList, refList);
			}//同模块查找
			else if (moduleVar.isString()) {
				refList.clear();
				inheritList.insert(std::make_pair(moduleName, className));
				return this->getVar(caller, moduleVar.toString(), classVar.toString(), variable,
					type, value, inheritList, refList);
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
	
	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.insert(caller);

	laf.flag = true;
	data = &(laf.data);
	return true;
}

uint8_t LAFConfigs::HexToDec(const juce::juce_wchar& s) const
{
	if (s >= '0' && s <= '9') {
		return s - '0';
	}
	else if (s >= 'a' && s <= 'f') {
		return 9 + s - 'a';
	}
	else if (s >= 'A' && s <= 'F') {
		return 9 + s - 'A';
	}
	else {
		return 0;
	}
}
