#pragma once
#include <JuceHeader.h>

class TabList final : public juce::Component
{
public:
	TabList();
	~TabList() override = default;

	bool newProj(const juce::String& name, const juce::String& path);
	bool copyProj(const juce::String& name, const juce::String& path,
		const juce::String& nameSrc, const juce::String& pathSrc);
	bool openProj(const juce::String& name, const juce::String& path);
	bool wannaClose();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabList)
};