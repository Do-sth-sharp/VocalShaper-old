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

public:
	void listenColorChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenSMChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);

private:
	struct Colors final
	{
		juce::Colour border;

		juce::Colour text_trackHeadName;

		juce::Colour text_MSButton;
		juce::Colour background_MSButton;
		juce::Colour text_MSButton_highlight;
	}colors;//界面颜色
	struct Size final
	{
		double width_trackHead;

		double width_trackHeadLeftMargin;
		double width_trackHeadRightMargin;
		double height_trackHeadTopMargin;
		double height_trackHeadBottomMargin;

		double height_trackHeadTitleAndButtonLine;
		double height_trackHeadNameText;

		double width_trackHeadColorLightLeftMargin;
		double width_trackHeadLightNameSplit;
		double width_trackHeadNameButtonSplit;
		double width_trackHeadButtonSplit;

		double height_trackHeadLineSplit;
		double height_trackHeadControlLine;

		double width_trackHeadLinkCurveSplit;

	}sizes;//控件大小
	struct Scale final
	{
		double height_curveByTrack;

		double height_trackHeadColorLight;
		double height_trackHeadMSButton;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* SMButton;
	}lafs;//控件样式

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	std::function<const juce::String(const juce::String&)> tr;

	const vocalshaper::Track* track = nullptr;
	int index = -1;
	bool isMaster = false;
	bool curveShown = false;

	std::function<void(double, double)> wheelChangeHMethod;
	std::function<void(double, double)> wheelChangeWithCtrlHMethod;
	std::function<void(double, double)> wheelChangeVMethod;
	std::function<void(double, double)> wheelChangeWithCtrlVMethod;
	std::function<void(const vocalshaper::Track*, bool)> showCurveMethod;

	std::unique_ptr<juce::TextButton> mButton = nullptr, sButton = nullptr;
	std::unique_ptr<juce::TextButton> linkButton = nullptr;
	std::unique_ptr<juce::DrawableButton> curveButton = nullptr;

	void setColor(juce::Colour color);

	void sendSolo(bool solo);
	void sendMute(bool mute);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackView)
};