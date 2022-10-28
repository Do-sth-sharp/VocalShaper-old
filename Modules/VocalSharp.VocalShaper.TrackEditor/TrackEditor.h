#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "TimeRuler.h"
#include "VScroller.h"
#include "HScroller.h"

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
		const std::function<void(double)>& setCurrentPositionFunc,
		const std::function<void(double, double)>& setLoopRangeFunc,
		const std::function<void(double, double)>& setHorizontalViewPortFunc,
		const std::function<void(double, double)>& setVerticalViewPortFunc
	) override;
	void setTrackViewMethods(
		std::function<void(double, double)> setHViewPortFunc,
		std::function<void(double, double)> setVViewPortFunc
	) override;

public:
	void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	void setEditMode(bool editMode) override;
	void setToolID(uint8_t toolID) override;
	void trackChanged(int trackID) override;
	void setTotalLength(double totalLength) override;
	void setCurrentPosition(double currentTime) override;
	void setFollowState(bool follow) override;
	void setLoopRange(double startTime, double endTime) override;
	void setHorizontalViewPort(double startTime, double endTime) override;
	void setVerticalViewPort(double bottomPitch, double topPitch) override;
	void setHViewPort(double startTime, double endTime) override;
	void setVViewPort(double bottomTrack, double topTrack) override;
	void setAdsorb(vocalshaper::AdsorbState state) override;
	void setGrid(vocalshaper::GridState state) override;

public:
	void changeHViewPort(double startTime, double endTime);
	void changeVViewPort(double bottomTrack, double topTrack);

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
	std::unique_ptr<HScroller> hScroller = nullptr;

	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;

	double startTimeTemp, endTimeTemp;
	double bottomVTrackTemp = 1., topVTrackTemp = 0.;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackEditor)
};