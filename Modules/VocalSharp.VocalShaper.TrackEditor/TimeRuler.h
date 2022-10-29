#pragma once
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
	void setTotalLength(double totalLength) override;
	void setCurrentPosition(double currentTime) override;
	void setLoopRange(double startTime, double endTime) override;
	void setAdsorb(vocalshaper::AdsorbState state) override;
	void setGrid(vocalshaper::GridState state) override;
	void setHViewPort(double startTime, double endTime) override;
	void setVViewPort(double bottomTrack, double topTrack) override;

private:
	struct Colors final
	{
		juce::Colour background_timeRuler;
		juce::Colour text_timeRuler;

		juce::Colour timeRuler_loopArea;
		juce::Colour timeRuler_loopJudgeArea;

		juce::Colour timeRuler_label_on;
		juce::Colour timeRuler_label_off;

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

		double width_timeRuler_loopJudgeArea;

		double height_timeRuler_labelBottomMargin;
		double height_timeRuler_label;
	}sizes;//控件大小
	struct Scales final
	{
		double height_timeRuler_barText;
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;

	vocalshaper::ProjectProxy* project = nullptr;
	double startTime, endTime;
	double loopStartTime, loopEndTime;
	double currentTime;
	double labelEditingTime = -1;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;

	vocalshaper::AdsorbState adsorb = vocalshaper::AdsorbState::Adsorb1Beat;
	vocalshaper::GridState grid = vocalshaper::GridState::Grid1Beat;
};