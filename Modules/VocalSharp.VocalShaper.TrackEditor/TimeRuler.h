﻿#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TimeRuler final : public vocalshaper::EditorBase
{
public:
	TimeRuler();
	~TimeRuler() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

public:
	void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	void setEditMode(bool editMode) override;
	void setToolID(uint8_t toolID) override;
	void trackChanged(int trackID) override;
	void setTotalLength(vocalshaper::ProjectTime totalLength) override;
	void setCurrentPosition(vocalshaper::ProjectTime currentTime) override;
	void setLoopRange(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) override;
	void setAdsorb(vocalshaper::AdsorbState state) override;
	void setGrid(vocalshaper::GridState state) override;
	void setHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) override;
	void setVViewPort(double bottomTrack, double topTrack) override;

private:
	struct Colors final
	{
		juce::Colour background_timeRuler;
		juce::Colour text_timeRuler;

		juce::Colour border;
		juce::Colour cursor;
	}colors;//界面颜色
	struct Size final
	{
		double height_borderTop;

		double width_cursor;

		double height_timeRuler_barLine;
		double height_timeRuler_beatLine;
		double width_timeRuler_barLine;
		double width_timeRuler_beatLine;

		double width_timeRuler_minLineSpace;

		double width_timeRuler_barTextLeftMargin;
		double width_timeRuler_barTextRightMargin;
	}sizes;//控件大小
	struct Scales final
	{
		double height_timeRuler_barText;
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;

	vocalshaper::ProjectProxy* project = nullptr;
	vocalshaper::ProjectTime startTime, endTime;
	vocalshaper::ProjectTime loopStartTime, loopEndTime;
	vocalshaper::ProjectTime currentTime;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;

	vocalshaper::AdsorbState adsorb = vocalshaper::AdsorbState::Adsorb1Beat;
	vocalshaper::GridState grid = vocalshaper::GridState::Grid1Beat;
};