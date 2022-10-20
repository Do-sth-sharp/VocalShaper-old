#include "GlobalConfigs.h"
#include <libJModule.h>

bool GlobalConfigs::getReference(const juce::String& caller, const juce::String& fileName, juce::var*& ptr)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	return this->load(caller, fileName, ptr);
}

void GlobalConfigs::releaseAll()
{
	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.clear();

	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	this->list.clear();
}

void GlobalConfigs::close(const juce::String& caller)
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

bool GlobalConfigs::load(const juce::String& caller, const juce::String& fileName, juce::var*& data)
{
	ConfigFile& config = this->list[fileName];
	//如果配置表存在则获取，不存在则建立
	
	config.modules.insert(caller);//调用模块名入集合
	
	if (config.flag) {
		data = &(config.data);
		return true;
	}//配置表存在

	juce::String filePath =
		juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
		.getParentDirectory().getFullPathName() + "/config/" + fileName + ".json";//配置表文件位置
	juce::File configFile(filePath);
	if (!configFile.existsAsFile()) {
		this->list.erase(fileName);
		data = nullptr;
		return false;
	}//文件不存在
	
	config.data = juce::JSON::parse(configFile);
	if (!config.data.isObject()) {
		this->list.erase(fileName);
		data = nullptr;
		return false;
	}//文件格式不正确

	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.insert(caller);
	
	config.flag = true;
	data = &(config.data);
	return true;
}
