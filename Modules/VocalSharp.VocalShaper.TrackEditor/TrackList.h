#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "TrackView.h"

class TrackList final : public vocalshaper::EditorBase
{
public:
	TrackList(std::function<void(double, double)> wheelChangeHMethod,
		std::function<void(double, double)> wheelChangeWithCtrlHMethod,
		std::function<void(double, double)> wheelChangeVMethod,
		std::function<void(double, double)> wheelChangeWithCtrlVMethod,
		std::function<void(const vocalshaper::Track*, bool)> showCurveMethod,
		std::function<bool(const vocalshaper::Track*)> curveIsShownMethod);

	void resized() override;
	void paint(juce::Graphics& g) override;
	void paintOverChildren(juce::Graphics& g) override;

	void refreshList();
	void resizeList();

public:
	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details) override;

public:
	void setProjectCallback(const vocalshaper::ProjectProxy* ptr) override;
	void setTrackIDCallback(int trackID) override;
	void setCurrentPositionCallback(double currentTime) override;
	void setLoopRangeCallback(double startTime, double endTime) override;
	void setHViewPortCallback(double startTime, double endTime) override;
	void setVViewPortCallback(double bottomTrack, double topTrack) override;
	void setGridCallback(vocalshaper::GridState state) override;

private:
	void listenColorChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenSMChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenLinkChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);

private:
	struct Colors final
	{
		juce::Colour border;
		juce::Colour cursor;

		juce::Colour fill_trackList_timeUnselect;
	}colors;//界面颜色
	struct Size final
	{
		double width_trackHead;

		double width_cursor;

		double height_borderTop;
	}sizes;//控件大小
	struct Scale final
	{
		double height_curveByTrack;
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	std::function<void(double, double)> wheelChangeHMethod;
	std::function<void(double, double)> wheelChangeWithCtrlHMethod;
	std::function<void(double, double)> wheelChangeVMethod;
	std::function<void(double, double)> wheelChangeWithCtrlVMethod;

	std::function<void(const vocalshaper::Track*, bool)> showCurveMethod;
	std::function<bool(const vocalshaper::Track*)> curveIsShownMethod;

	juce::OwnedArray<TrackView> trackList;
	juce::ReadWriteLock listLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackList)
};