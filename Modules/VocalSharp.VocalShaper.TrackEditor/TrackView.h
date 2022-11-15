#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackView final : public vocalshaper::EditorBase
{
public:
	TrackView(std::function<void(double, double)> wheelChangeHMethod,
		std::function<void(double, double)> wheelChangeWithCtrlHMethod,
		std::function<void(double, double)> wheelChangeVMethod,
		std::function<void(double, double)> wheelChangeWithCtrlVMethod,
		std::function<void(const vocalshaper::Track*, bool)> showCurveMethod);

	void setTrack(const vocalshaper::Track* track, int index = -1, bool isMaster = false);
	const vocalshaper::Track* getTrack();
	int getIndex();
	bool getIsMaster();

	void setCurveShown(bool show);
	bool isCurveShown();

	int getCurveSize();

	void resized() override;
	void paint(juce::Graphics& g) override;

public:
	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details) override;

private:
	struct Colors final
	{
		juce::Colour border;
	}colors;//界面颜色
	struct Size final
	{
		double width_trackHead;
		double width_trackColorLabel;
	}sizes;//控件大小
	struct Scale final
	{
		double height_curveByTrack;
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	const vocalshaper::Track* track = nullptr;
	int index = -1;
	bool isMaster = false;
	bool curveShown = false;

	std::function<void(double, double)> wheelChangeHMethod;
	std::function<void(double, double)> wheelChangeWithCtrlHMethod;
	std::function<void(double, double)> wheelChangeVMethod;
	std::function<void(double, double)> wheelChangeWithCtrlVMethod;
	std::function<void(const vocalshaper::Track*, bool)> showCurveMethod;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackView)
};