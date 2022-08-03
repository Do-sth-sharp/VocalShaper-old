#pragma once
#include <JuceHeader.h>

class Configs final
{
public:
	Configs() = default;
	~Configs() = default;
	
	bool getReference(const juce::String& caller, const juce::String& fileName, juce::var*& ptr);
	
	void releaseAll();//全部释放
	void release(const juce::String& moduleName);//释放某一模块名下的全部表

private:
	bool load(const juce::String& caller, const juce::String& fileName, juce::var*& ptr);
	
private:
	std::map<std::pair<juce::String, juce::String>, juce::var> list;
	juce::SpinLock lock;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Configs)
};