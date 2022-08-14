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
		std::unique_ptr<juce::LookAndFeel> startMenuTextEditor;
		std::unique_ptr<juce::LookAndFeel> startMenuListBox;

		std::unique_ptr<juce::LookAndFeel> mainMenuButton;
	}lafs;


public:
	juce::LookAndFeel* getStartMenuButtonLAF(int fontHeight);
	juce::LookAndFeel* getStartMenuTextEditorLAF();
	juce::LookAndFeel* getStartMenuListBoxLAF();

	juce::LookAndFeel* getMainMenuButtonLAF();
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LAFFactory)
};