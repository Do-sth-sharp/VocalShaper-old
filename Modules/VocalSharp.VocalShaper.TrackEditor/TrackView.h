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
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseMove(const juce::MouseEvent& event) override;
	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;
	void mouseExit(const juce::MouseEvent& event) override;

public:
	void listenColorChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenNameChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenSMChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenLinkChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);

private:
	struct Colors final
	{
		juce::Colour border;

		juce::Colour text_trackNameEditor;
		juce::Colour text_trackNameEditor_empty;
		juce::Colour background_trackNameEditor;
		juce::Colour text_trackNameEditor_highlight;
		juce::Colour background_trackNameEditor_highlight;
		juce::Colour caret_trackNameEditor;

		juce::Colour text_MSButton;
		juce::Colour background_MSButton;
		juce::Colour text_MSButton_highlight;

		juce::Colour text_trackViewLinkButton;
		juce::Colour background_trackViewLinkButton;

		juce::Colour icon_trackViewShowCurveButton;
		juce::Colour background_trackViewShowCurveButton;
		juce::Colour icon_trackViewShowCurveButton_highlight;
		juce::Colour background_trackViewShowCurveButton_highlight;
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
		std::unique_ptr<juce::LookAndFeel> SMButton;
		std::unique_ptr<juce::LookAndFeel> linkButton;
		std::unique_ptr<juce::LookAndFeel> showCurveButton;
		std::unique_ptr<juce::LookAndFeel> textEditor;
	}lafs;//控件样式

	void initUIConfigAndIcon();

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

	std::unique_ptr<juce::TextEditor> nameEditor = nullptr;
	std::unique_ptr<juce::TextButton> mButton = nullptr, sButton = nullptr;
	std::unique_ptr<juce::TextButton> linkButton = nullptr;
	std::unique_ptr<juce::DrawableButton> curveButton = nullptr;
	std::unique_ptr<juce::Drawable> iconCurve = nullptr, iconCurveHighlight = nullptr;

	void setColor(juce::Colour color);
	void setTrackName(const juce::String& name);
	void setTrackType(const vocalshaper::Track* track);

	void sendSolo(bool solo);
	void sendMute(bool mute);
	void sendName(const juce::String& name);
	void sendColor(juce::Colour color);

	void showLinkMenu();
	void showCurve(bool show);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackView)
};