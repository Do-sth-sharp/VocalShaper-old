#pragma once
#include <JuceHeader.h>
#include "TopLevelEditorBase.h"
#include "StatusBar.h"

class BottomEditor final : public TopLevelEditorBase
{
public:
	BottomEditor();
	~BottomEditor() override = default;

	void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	void setEditMode(bool editMode) override;
	void setToolID(uint8_t toolID) override;

	bool isActive();
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> getCopy() override;
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> getCut() override;
	bool wannaDelete() override;
	bool wannaCopy() override;
	bool wannaSelectAll() override;
	bool wannaPaste(juce::OwnedArray<vocalshaper::SerializableProjectStructure> list) override;
	bool wannaPaste(const juce::StringArray& list) override;
	int showClipBoard(const juce::StringArray& list) override;

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Size final : public TopLevelEditorBase::Size
	{
		double height_statusBar;
	}sizes;//¿Ø¼þ´óÐ¡

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	std::unique_ptr<StatusBar> statusBar;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomEditor)
};