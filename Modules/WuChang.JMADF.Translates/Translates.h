#pragma once
#include <JuceHeader.h>

class Translates final
{
public:
	Translates() = default;
	~Translates() = default;
	
	bool getReference(const juce::String& caller, const juce::String& fileName, juce::var*& ptr);
	
	void releaseAll();//ȫ���ͷ�
	void release(const juce::String& moduleName);//�ͷ�ĳһģ�����µ�ȫ����

	const juce::String tr(const juce::String& caller, const juce::String& str);
	const juce::String tr(const juce::var* list, const juce::String& str);
	void setCurrentLang(const juce::String& lang);
	const juce::String getCurrentLang();
private:
	bool load(const juce::String& caller, const juce::String& fileName, juce::var*& ptr);
	
private:
	std::map<std::pair<juce::String, juce::String>, juce::var> list;
	juce::SpinLock lock;
	juce::String currentLang;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Translates)
};