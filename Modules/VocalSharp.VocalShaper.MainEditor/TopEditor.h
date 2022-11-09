#pragma once
#include <JuceHeader.h>
#include "TopLevelEditorBase.h"

class TopEditor final : public TopLevelEditorBase
{
public:
	TopEditor();
	~TopEditor() override = default;

	bool isActive() override;
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> getCopy() override;
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> getCut() override;
	bool wannaDelete() override;
	bool wannaCopy() override;
	bool wannaSelectAll() override;
	bool wannaPaste(juce::OwnedArray<vocalshaper::SerializableProjectStructure> list) override;
	bool wannaPaste(const juce::StringArray& list) override;
	int showClipBoard(const juce::StringArray& list) override;

	void resized() override;

private:
	struct Size final : public TopLevelEditorBase::Size
	{
	}sizes;//控件大小

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	void initCommandID();
	void initCommandFunction();
	void initCommandFlagHook();

	vocalshaper::EditorBase* topEditor = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopEditor);
};