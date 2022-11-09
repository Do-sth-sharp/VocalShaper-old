#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "TimeRuler.h"
#include "VScroller.h"
#include "HScroller.h"
#include "TrackList.h"

class TrackEditor final : public vocalshaper::EditorBase
{
public:
	TrackEditor();
	~TrackEditor() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

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
	std::unique_ptr<TrackList> tracks = nullptr;

	double startTimeTemp = 0., endTimeTemp = 0.;
	double bottomVTrackTemp = 0., topVTrackTemp = 0.;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackEditor)
};