#pragma once
#include <JuceHeader.h>

class LAFFactory final
{
public:
	LAFFactory() = default;
	~LAFFactory() = default;

private:
	struct LAF final
	{
		std::unique_ptr<juce::LookAndFeel> startMenuButton;
	}lafs;


public:
	juce::LookAndFeel* getStartMenuButtonLAF(int fontHeight);
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LAFFactory)
};