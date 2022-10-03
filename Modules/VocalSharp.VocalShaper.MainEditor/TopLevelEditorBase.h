#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "ToolBar.h"

class TopLevelEditorBase : public vocalshaper::EditorBase
{
public:
	TopLevelEditorBase();
	virtual ~TopLevelEditorBase() override = default;

	virtual void resized() override;

protected:
	struct Size
	{
		double width_toolBar;
	};//控件大小

	std::unique_ptr<ToolBar> toolBar;

private:
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	Size sizes;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopLevelEditorBase)
};