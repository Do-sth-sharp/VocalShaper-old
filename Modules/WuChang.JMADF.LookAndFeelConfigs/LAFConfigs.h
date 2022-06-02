#pragma once
#include <JuceHeader.h>

class LAFConfigs final
{
public:
	LAFConfigs() = default;
	~LAFConfigs() = default;
	
	bool getNum(const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, double& value);
	bool getString(const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, juce::String& value);
	bool getBool(const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, bool& value);
	
	void releaseAll();
	void release(const juce::String& moduleName);
	
	void close(const juce::String& caller);
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LAFConfigs)
};