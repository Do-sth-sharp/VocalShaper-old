#pragma once
#include <JuceHeader.h>
#include "TopLevelEditorBase.h"

class TopEditor final : public TopLevelEditorBase
{
public:
	TopEditor();
	~TopEditor() override = default;

	void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	void trackChanged(int trackID) override;
	void setEditMode(bool editMode) override;
	void setToolID(uint8_t toolID) override;
	void setHorizontalViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) override;
	void setVerticalViewPort(double bottomPitch, double topPitch) override;
	void setTotalLength(vocalshaper::ProjectTime totalLength) override;
	void setCurrentPosition(vocalshaper::ProjectTime currentTime) override;
	void setAdsorb(vocalshaper::AdsorbState state) override;
	void setGrid(vocalshaper::GridState state) override;

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

	void setMethods(
		const std::function<void(int)>& setCurrentTrackFunc,
		const std::function<void(void)>& refreshTotalTimeFunc,
		const std::function<void(vocalshaper::ProjectTime)>& setCurrentPositionFunc,
		const std::function<void(vocalshaper::ProjectTime, vocalshaper::ProjectTime)>& setHorizontalViewPortFunc,
		const std::function<void(double, double)>& setVerticalViewPortFunc
	) override;

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