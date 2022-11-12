#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "LabelEditor.h"
#include "TimeValueViewer.h"

class TimeRuler final : public vocalshaper::EditorBase
{
public:
	TimeRuler(std::function<void(double, double)> wheelChangeMethod,
		std::function<void(double, double)> wheelChangeWithCtrlMethod);
	~TimeRuler() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

public:
	void setProjectCallback(const vocalshaper::ProjectProxy* ptr) override;
	void setEditModeCallback(bool editMode) override;
	void setTrackIDCallback(int trackID) override;
	void setTotalLengthCallback(double totalLength) override;
	void setCurrentPositionCallback(double currentTime) override;
	void setLoopRangeCallback(double startTime, double endTime) override;
	void setAdsorbCallback(vocalshaper::AdsorbState state) override;
	void setGridCallback(vocalshaper::GridState state) override;
	void setHViewPortCallback(double startTime, double endTime) override;

public:
	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseMove(const juce::MouseEvent& event) override;
	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;
	void mouseExit(const juce::MouseEvent& event) override;
	void mouseDoubleClick(const juce::MouseEvent& event) override;

public:
	void listenLabelChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);

public:
	void onCopy(vocalshaper::SPSList& list) override;
	void onDelete() override;
	void onUnselectAll() override;
	void onPaste(const vocalshaper::SPSList& list) override;

private:
	struct Colors final
	{
		juce::Colour background_timeRuler;
		juce::Colour text_timeRuler;

		juce::Colour timeRuler_loopArea;
		juce::Colour timeRuler_loopJudgeArea;

		juce::Colour timeRuler_label_on;
		juce::Colour timeRuler_label_off;
		juce::Colour timeRuler_labelBorderSelected;

		juce::Colour background_timeViewer;
		juce::Colour border_timeViewer;

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
		double height_timeRuler_labelBorderSelected;

		double width_labelEditor;
		double height_labelEditor;
		double width_labelEditorCalloutArrow;
		double width_labelEditorCalloutBorder;
		double width_labelEditorCalloutCorner;

		double width_timeViewerBorder;
		double width_timeViewerCorner;
	}sizes;//控件大小
	struct Scales final
	{
		double height_timeRuler_barText;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* timeViewerBubble;
	}lafs;//控件样式

	void showLabelEditor(int labelIndex, juce::Rectangle<int> place);

	void initCommandID();

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;
	std::function<const juce::String(const juce::String&)> tr;

	std::function<void(double, double)> wheelChangeMethod;
	std::function<void(double, double)> wheelChangeWithCtrlMethod;

	std::unique_ptr<LabelEditorCallOutBox> labelEditor = nullptr;
	std::unique_ptr<TimeValueViewer> timeValue = nullptr;

	int timeViewerBorderSize = 1;
	float timeViewerCornerSize = 5.f;

	double labelEditingTime = -1;
	int labelEditingIndex = -1;
	int labelSelectedIndex = -1;
	double timePressed = -1;
	double totalTime = 0.;

	enum class RulerState {
		Normal,		//未更改
		LoopSP,		//更改循环起点
		LoopEP,		//更改循环终点
		Label,		//更改标签位置
		Cursor		//更改播放指针位置
	}rulerState = RulerState::Normal;

	int cutCommandID = -1, copyCommandID = -1, pasteCommandID = -1,
		createCopyCommandID= -1,
		copyToSystemCommandID = -1, pasteFromSystemCommandID = -1;
	juce::ApplicationCommandManager* commandManager = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeRuler)
};