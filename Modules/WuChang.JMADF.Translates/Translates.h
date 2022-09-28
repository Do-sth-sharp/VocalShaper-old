#pragma once
#include <JuceHeader.h>

class Translates final
{
public:
	Translates() = default;
	~Translates() = default;
	
	bool getReference(const juce::String& caller, const juce::String& fileName, juce::var*& ptr);

	void releaseAll();//全部释放
	void release(const juce::String& moduleName);//释放某一模块名下的全部表

	const juce::String tr(const juce::String& caller, const juce::String& str);
	const juce::String trFast(const juce::String& caller, const juce::var* list, const juce::String& str);
	
	void setCurrentLang(const juce::String& lang);
	const juce::String getCurrentLang();
	void setDefaultLang(const juce::StringArray& langArray);

private:
	const juce::String tr(const juce::var* list, const juce::String& str);
	bool load(const juce::String& caller, const juce::String& fileName, juce::var*& ptr);
	const juce::String trDefault(const juce::String& caller, const juce::String& str);
	
private:
	std::map<std::pair<juce::String, juce::String>, juce::var> list;
	juce::SpinLock lock;
	juce::String currentLang;
	juce::StringArray defaultList;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Translates)
};