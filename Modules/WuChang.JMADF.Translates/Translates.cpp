#include "Translates.h"
#include <libJModule.h>

bool Translates::getReference(const juce::String& caller, const juce::String& fileName, juce::var*& ptr)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	juce::var* result = nullptr;

	if (!this->load(caller, fileName, result)) {
		return false;
	}
	
	ptr = result;
	return true;
}

void Translates::releaseAll()
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	this->list.clear();
}

void Translates::release(const juce::String& moduleName)
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

bool Translates::load(const juce::String& caller, const juce::String& fileName, juce::var*& ptr)
{
	auto laf = this->list.find(std::make_pair(caller, fileName));
	//如果配置表存在则获取，不存在则建立
	
	if (laf != this->list.end()) {
		ptr = &((*laf).second);
		return true;
	}//配置表存在
	
	const jmadf::ModuleInfo* info = jmadf::FindModule(caller);
	if (!info) {
		ptr = nullptr;
		return false;
	}//模块不存在

	juce::String filePath = info->path + "/.tr/" + fileName + ".json";//配置表文件位置
	juce::File configFile(filePath);
	if (!configFile.existsAsFile()) {
		ptr = nullptr;
		return false;
	}//文件不存在
	
	juce::var data = juce::JSON::parse(configFile);
	if (!data.isObject()) {
		ptr = nullptr;
		return false;
	}//文件格式不正确
	
	auto& temp = this->list[std::make_pair(caller, fileName)];
	temp = data;
	ptr = &(temp);
	return true;
}

const juce::String Translates::trDefault(const juce::String& caller, const juce::String& str)
{
	juce::String result;
	juce::var* trans = nullptr;
	for (auto& i : this->defaultList) {
		this->getReference(caller, i, trans);
		if ((result = this->tr(trans, str)).isNotEmpty()) {
			return result;
		}
	}
	return result;
}

const juce::String Translates::tr(const juce::String& caller, const juce::String& str)
{
	juce::var* trans = nullptr;
	this->getReference(caller, this->currentLang, trans);

	return this->trFast(caller, trans, str);
}

const juce::String Translates::trFast(const juce::String& caller, const juce::var* list, const juce::String& str)
{
	juce::String result;
	return (result = this->tr(list, str)).isEmpty() 
		? ((result = this->trDefault(caller, str)).isEmpty() ? str : result) 
		: result;
}

const juce::String Translates::tr(const juce::var* list, const juce::String& str)
{
	if (list == nullptr) {
		return juce::String();
	}
	if (!(*list)[str.toStdString().c_str()].isString()) {
		return juce::String();
	}
	return (*list)[str.toStdString().c_str()].toString();
}

void Translates::setCurrentLang(const juce::String& lang)
{
	this->currentLang = lang;
}

const juce::String Translates::getCurrentLang()
{
	return this->currentLang;
}

void Translates::setDefaultLang(const juce::StringArray& langArray)
{
	this->defaultList = langArray;
}
