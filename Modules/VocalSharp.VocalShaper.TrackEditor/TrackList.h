#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackList final : public vocalshaper::EditorBase
{
public:
	TrackList(std::function<void(double, double)> wheelChangeHMethod,
		std::function<void(double, double)> wheelChangeWithCtrlHMethod,
		std::function<void(double, double)> wheelChangeVMethod,
		std::function<void(double, double)> wheelChangeWithCtrlVMethod);

	void resized() override;
	void paint(juce::Graphics& g) override;

public:
	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details) override;

public:
	void setProjectCallback(const vocalshaper::ProjectProxy* ptr) override;
	void setEditModeCallback(bool editMode) override;
	void setToolIDCallback(uint8_t toolID) override;
	void setTrackIDCallback(int trackID) override;
	void setTotalLengthCallback(double totalLength) override;
	void setCurrentPositionCallback(double currentTime) override;
	void setLoopRangeCallback(double startTime, double endTime) override;
	void setHorizontalViewPortCallback(double startTime, double endTime) override;
	void setVerticalViewPortCallback(double bottomPitch, double topPitch) override;
	void setHViewPortCallback(double startTime, double endTime) override;
	void setVViewPortCallback(double bottomTrack, double topTrack) override;
	void setAdsorbCallback(vocalshaper::AdsorbState state) override;
	void setGridCallback(vocalshaper::GridState state) override;

private:
	struct Size final
	{
		double width_trackHead;
	}sizes;//控件大小

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	std::function<void(double, double)> wheelChangeHMethod;
	std::function<void(double, double)> wheelChangeWithCtrlHMethod;
	std::function<void(double, double)> wheelChangeVMethod;
	std::function<void(double, double)> wheelChangeWithCtrlVMethod;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackList)
};