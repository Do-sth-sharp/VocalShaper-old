#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackEditor final : public vocalshaper::EditorBase
{
public:
	TrackEditor();
	~TrackEditor() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

public:
	void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	void setEditMode(bool editMode) override;
	void setToolID(uint8_t toolID) override;
	void trackChanged(int trackID) override;
	void setTotalLength(vocalshaper::ProjectTime totalLength) override;
	void setCurrentPosition(vocalshaper::ProjectTime currentTime) override;

private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour background_rulerHead;
	}colors;//界面颜色
	struct Size final
	{
		double width_trackHead;
		double height_timeRuler;
	}sizes;//控件大小
	struct Scales final
	{
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;

	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackEditor)
};