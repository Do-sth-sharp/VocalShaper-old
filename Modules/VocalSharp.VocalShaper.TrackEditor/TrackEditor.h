﻿#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "TimeRuler.h"

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
	void setAdsorb(vocalshaper::AdsorbState state) override;
	void setGrid(vocalshaper::GridState state) override;

private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour border;

		juce::Colour background_rulerHead;
		juce::Colour background_timeRuler;
	}colors;//界面颜色
	struct Size final
	{
		double width_trackHead;
		double height_timeRuler;
		double width_verticalScroller;

		double height_borderTop;
		double width_borderRight;
	}sizes;//控件大小
	struct Scales final
	{
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;

	std::unique_ptr<TimeRuler> timeRuler = nullptr;

	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackEditor)
};