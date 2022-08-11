#pragma once
#include <JuceHeader.h>

class GlobalConfigs final
{
public:
	GlobalConfigs() = default;
	~GlobalConfigs() = default;

	bool getReference(const juce::String& caller, const juce::String& fileName, juce::var*& ptr);

	void releaseAll();//无论如何全部释放
	
	void close(const juce::String& caller);//某一模块关闭并清理

private:
	class ConfigFile final
	{
	public:
		juce::var data;
		std::set<juce::String> modules;
		bool flag = false;
		
	private:
		JUCE_LEAK_DETECTOR(ConfigFile)
	};
	
private:
	bool load(const juce::String& caller, const juce::String& fileName, juce::var*& data);

private:
	std::map<juce::String, ConfigFile> list;
	juce::SpinLock lock;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalConfigs)
};