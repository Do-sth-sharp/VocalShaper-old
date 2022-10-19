#pragma once
#include <JuceHeader.h>
#include "TopEditor.h"
#include "BottomEditor.h"
#include <libVocalShaper.h>

class EditorComponent final : public juce::Component
{
public:
	EditorComponent();
	~EditorComponent() override;

public:
	//被命令调用
	void setTrackOpen(bool trackOpen);
	bool isTrackOpen();

	//被回调调用
	void projectChanged(const vocalshaper::ProjectProxy* ptr);

	//被命令调用
	void undo();
	void redo();
	void cut();
	void copy();
	void paste();
	void clipBoard();
	void cleanClipBoard();
	void createCopy();
	void delete_();
	void copyToSystem();
	void pasteFromSystem();
	void selectAll();

	//被命令调用
	bool couldUndo();
	bool couldRedo();
	bool couldCut();
	bool couldCopy();
	bool couldPaste();
	bool couldCleanClipBoard();
	bool couldCreateCopy();
	bool couldDelete();
	bool couldCopyToSystem();
	bool couldPasteFromSystem();
	bool couldSelectAll();

	//被命令调用
	void lastTrack();
	void nextTrack();
	void switchTrack();

	//被命令调用
	bool couldLastTrack();
	bool couldNextTrack();
	bool couldSwitchTrack();

	//外部/内部调用
	void setCurrentTrack(int trackID);
	void refreshTotalLength();
	void setCurrentPosition(vocalshaper::ProjectTime currentTime);
	void setHorizontalViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime);
	void setVerticalViewPort(double bottomPitch, double topPitch);

	//外部调用
	void setAdsorb(vocalshaper::AdsorbState state);
	void setGrid(vocalshaper::GridState state);

private:
	//内部调用
	void trackChanged(int trackID);
	void totalLengthChanged(vocalshaper::ProjectTime totalLength);
	void currentPositionChanged(vocalshaper::ProjectTime currentTime);
	void horizontalViewPortChanged(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime);
	void verticalViewPortChanged(double bottomPitch, double topPitch);

public:
	//被命令调用
	bool isEditMode();
	void setEditMode(bool editMode);
	uint8_t getToolID();
	void setToolID(uint8_t toolID);

public:
	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour stretchableBar;
	}colors;//界面颜色
	struct Size final
	{
		double height_stretchableBar;
	}sizes;//控件大小
	struct Scales final
	{
		double height_top_max;
		double height_top_min;
		double height_top_prefer;
		double height_bottom_max;
		double height_bottom_min;
		double height_bottom_prefer;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* stretchableBar;
	}lafs;//控件样式

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;

	void initCommandID();
	void initCommandFunction();
	void initCommandFlagHook();

	std::unique_ptr<TopEditor> topEditor;
	std::unique_ptr<BottomEditor> bottomEditor;

	std::unique_ptr<juce::StretchableLayoutManager> slManager;
	std::unique_ptr<juce::StretchableLayoutResizerBar> slBar;

	bool trackOpenStateChanged = false;
	bool editModeFlag = false;
	uint8_t toolID = 1;

	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	static vocalshaper::ProjectTime countProjectTime(vocalshaper::ProjectProxy* ptr);
	static vocalshaper::ProjectTime countTrackTime(vocalshaper::ProjectProxy* ptr, int trackID);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorComponent)
};