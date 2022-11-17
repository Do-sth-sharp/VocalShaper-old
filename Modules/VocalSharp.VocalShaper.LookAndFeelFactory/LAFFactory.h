#pragma once
#include <JuceHeader.h>

class LAFFactory final
{
public:
	LAFFactory() = default;
	~LAFFactory() = default;

private:
	std::list<std::pair<juce::String, std::unique_ptr<juce::LookAndFeel>>> lafs;
	juce::CriticalSection lock;

public:
	juce::LookAndFeel* getStartMenuButtonLAF(const juce::String& caller, const std::function<int()>& fontHeightFunc);
	juce::LookAndFeel* getStartMenuTextEditorLAF(const juce::String& caller);
	juce::LookAndFeel* getStartMenuListBoxLAF(const juce::String& caller);

	juce::LookAndFeel* getMainMenuButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getTabCloseButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getPlayButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getPlayTextButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getToolButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getStretchableBarLAF(const juce::String& caller, juce::Colour mainColour);
	juce::LookAndFeel* getStatusButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getStatusTextButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getStatusRoundButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getStatusFlatTextButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getLabelEditorComboBoxLAF(const juce::String& caller);
	juce::LookAndFeel* getLabelEditorCodeEditorLAF(const juce::String& caller);
	juce::LookAndFeel* getLabelEditorResultLabelLAF(const juce::String& caller);
	juce::LookAndFeel* getLabelEditorButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getLabelEditorCallOutBoxLAF(const juce::String& caller, 
		const std::function<int()>& borderSizeFunc, const std::function<float()>& cornerSizeFunc,
		const juce::Colour colorBackground, const juce::Colour colorBorder);
	juce::LookAndFeel* getLabelEditorBubbleLAF(const juce::String& caller,
		const std::function<int()>& borderSizeFunc, const std::function<float()>& cornerSizeFunc);
	juce::LookAndFeel* getTrackViewSMButtonLAF(const juce::String& caller);
	juce::LookAndFeel* getTrackViewLinkButtonLAF(const juce::String& caller);

	void close(const juce::String& caller);

private:
	void addToList(const juce::String& caller, juce::LookAndFeel* laf);

	std::set<juce::String> mSet;
	juce::ReadWriteLock setLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LAFFactory)
};