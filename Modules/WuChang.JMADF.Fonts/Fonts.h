#pragma once
#include <JuceHeader.h>

class Fonts final
{
public:
	Fonts() = default;
	~Fonts() = default;
	
	bool getFont(const juce::String& caller, const juce::String& fontName, juce::Typeface::Ptr& ptr);
	bool getDefault(const juce::String& caller, juce::Typeface::Ptr& ptr);
	
	void releaseAll();//全部释放
	void close(const juce::String& caller);

	void setDefaultFont(const juce::String& font);
	const juce::String getDefaultFont();
private:
	struct Types final
	{
	public:
		juce::Typeface::Ptr font;
		std::set<juce::String> modules;
	};

	bool load(const juce::String& caller, const juce::String& fontName, Types*& type);
	
private:
	std::map<juce::String, Types> list;
	juce::SpinLock lock;
	juce::String defaultFont;

	std::set<juce::String> mSet;
	juce::ReadWriteLock setLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fonts)
};