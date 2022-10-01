#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "ToolBar.h"

class TopLevelEditorBase : public juce::Component
{
public:
	TopLevelEditorBase();
	virtual ~TopLevelEditorBase() override = default;

	virtual void projectChanged(const vocalshaper::ProjectProxy* ptr);
	virtual void setEditMode(bool editMode);
	virtual void setToolID(uint8_t toolID);
	virtual void trackChanged(int trackID);

	virtual bool isActive();
	virtual juce::OwnedArray<vocalshaper::SerializableProjectStructure> getCopy();
	virtual juce::OwnedArray<vocalshaper::SerializableProjectStructure> getCut();
	virtual bool wannaDelete();
	virtual bool wannaCopy();
	virtual bool wannaSelectAll();
	virtual bool wannaPaste(juce::OwnedArray<vocalshaper::SerializableProjectStructure> list);
	virtual bool wannaPaste(const juce::StringArray& list);
	virtual int showClipBoard(const juce::StringArray& list);

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