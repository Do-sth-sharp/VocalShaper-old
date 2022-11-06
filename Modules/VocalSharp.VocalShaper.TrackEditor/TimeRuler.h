#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>
#include "LabelEditor.h"

class TimeRuler final : public vocalshaper::EditorBase
{
public:
	TimeRuler(std::function<void(double, double)> wheelChangeMethod,
		std::function<void(double, double)> wheelChangeWithCtrlMethod);
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

		double width_labelEditor;
		double height_labelEditor;
		double width_labelEditorCalloutArrow;
		double width_labelEditorCalloutBorder;
		double width_labelEditorCalloutCorner;
	}sizes;//控件大小
	struct Scales final
	{
		double height_timeRuler_barText;
	}scales;//控件缩放

	void showLabelEditor(int labelIndex, juce::Rectangle<int> place);

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;
	std::function<const juce::String(const juce::String&)> tr;

	std::function<void(double, double)> wheelChangeMethod;
	std::function<void(double, double)> wheelChangeWithCtrlMethod;

	std::unique_ptr<LabelEditorCallOutBox> labelEditor = nullptr;

	vocalshaper::ProjectProxy* project = nullptr;
	double startTime, endTime;
	double loopStartTime, loopEndTime;
	double currentTime;
	double totalTime = 0;
	double labelEditingTime = -1;
	int labelEditingIndex = -1;
	double timePressed = -1;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;

	enum class RulerState {
		Normal,		//未更改
		LoopSP,		//更改循环起点
		LoopEP,		//更改循环终点
		Label,		//更改标签位置
		Cursor		//更改播放指针位置
	}rulerState = RulerState::Normal;

	vocalshaper::AdsorbState adsorb = vocalshaper::AdsorbState::Adsorb1Beat;
	vocalshaper::GridState grid = vocalshaper::GridState::Grid1Beat;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeRuler)
};