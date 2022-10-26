#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "TimeRuler.h"
#include "VScroller.h"

class TrackEditor final : public vocalshaper::EditorBase
{
public:
	TrackEditor();
	~TrackEditor() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

	void setMethods(
		const std::function<void(int)>& setCurrentTrackFunc,
		const std::function<void(void)>& refreshTotalTimeFunc,
		const std::function<void(vocalshaper::ProjectTime)>& setCurrentPositionFunc,
		const std::function<void(vocalshaper::ProjectTime, vocalshaper::ProjectTime)>& setLoopRangeFunc,
		const std::function<void(vocalshaper::ProjectTime, vocalshaper::ProjectTime)>& setHorizontalViewPortFunc,
		const std::function<void(double, double)>& setVerticalViewPortFunc
	) override;
	void setTrackViewMethods(
		std::function<void(vocalshaper::ProjectTime, vocalshaper::ProjectTime)> setHViewPortFunc,
		std::function<void(double, double)> setVViewPortFunc
	) override;

public:
	void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	void setEditMode(bool editMode) override;
	void setToolID(uint8_t toolID) override;
	void trackChanged(int trackID) override;
	void setTotalLength(vocalshaper::ProjectTime totalLength) override;
	void setCurrentPosition(vocalshaper::ProjectTime currentTime) override;
	void setFollowState(bool follow) override;
	void setLoopRange(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) override;
	void setHorizontalViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) override;
	void setVerticalViewPort(double bottomPitch, double topPitch) override;
	void setHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) override;
	void setVViewPort(double bottomPer, double topPer) override;
	void setAdsorb(vocalshaper::AdsorbState state) override;
	void setGrid(vocalshaper::GridState state) override;

public:
	void changeHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime);
	void changeVViewPort(double bottomPer, double topPer);

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
		double height_horizontalScroller;

		double height_borderTop;
		double width_borderRight;
	}sizes;//控件大小
	struct Scales final
	{
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;

	std::unique_ptr<TimeRuler> timeRuler = nullptr;
	std::unique_ptr<VScroller> vScroller = nullptr;

	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;

	vocalshaper::ProjectTime startTimeTemp, endTimeTemp;
	double bottomVPercentTemp = 1., topVPercentTemp = 0.;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackEditor)
};