#pragma once
#include <JuceHeader.h>

class LAFFactory final
{
public:
	LAFFactory() = default;
	~LAFFactory() = default;

private:
	juce::OwnedArray<juce::LookAndFeel> lafs;

public:
	juce::LookAndFeel* getStartMenuButtonLAF(int fontHeight);
	juce::LookAndFeel* getStartMenuTextEditorLAF();
	juce::LookAndFeel* getStartMenuListBoxLAF();

	juce::LookAndFeel* getMainMenuButtonLAF();
	juce::LookAndFeel* getTabCloseButtonLAF();
	juce::LookAndFeel* getPlayButtonLAF();
	juce::LookAndFeel* getPlayTextButtonLAF();
	juce::LookAndFeel* getToolButtonLAF();
	juce::LookAndFeel* getStretchableBarLAF(juce::Colour mainColour);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LAFFactory)
};