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
	std::unique_ptr<ToolBar> toolBar;

private:
	struct Size final
	{
		double width_toolBar;
	}sizes;//¿Ø¼þ´óÐ¡

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopLevelEditorBase)
};