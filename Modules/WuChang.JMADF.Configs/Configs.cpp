#include "Configs.h"
#include <libJModule.h>

bool Configs::getReference(const juce::String& caller, const juce::String& fileName, juce::var*& ptr)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	juce::var* result = nullptr;
	if (!this->load(caller, fileName, result)) {
		return false;
	}
	
	ptr = result;
	return true;
}

void Configs::releaseAll()
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	this->list.clear();
}

void Configs::release(const juce::String& moduleName)
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

bool Configs::load(const juce::String& caller, const juce::String& fileName, juce::var*& ptr)
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

	juce::String filePath = info->path + "/.config/" + fileName + ".json";//配置表文件位置
	juce::File configFile(filePath);
	if (!configFile.existsAsFile()) {
		ptr = nullptr;
		return false;
	}//文件不存在
	
	juce::var data = juce::JSON::parse(configFile);
	if (data.isVoid()) {
		ptr = nullptr;
		return false;
	}//文件格式不正确
	
	auto& temp = this->list[std::make_pair(caller, fileName)];
	temp = data;
	ptr = &(temp);
	return true;
}
