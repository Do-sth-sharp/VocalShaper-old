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
	//被命令调用，设置编辑器显示状态
	void setTrackOpen(bool trackOpen);
	bool isTrackOpen();

	//被回调调用，设置当前工程
	void projectChanged(const vocalshaper::ProjectProxy* ptr);

	//被命令调用，执行工程操作
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

	//被命令调用，检查是否可以进行工程操作
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

	//被命令调用，切换当前轨道
	void lastTrack();
	void nextTrack();
	void switchTrack();

	//被命令调用，检查是否可以切换轨道
	bool couldLastTrack();
	bool couldNextTrack();
	bool couldSwitchTrack();

	//外部/内部调用，申请更改状态
	void setCurrentTrack(int trackID);
	void refreshTotalLength();
	void setCurrentPosition(double currentTime);
	void setLoopRange(double startTime, double endTime);
	void setHorizontalViewPort(double startTime, double endTime);
	void setVerticalViewPort(double bottomPitch, double topPitch);

	//外部调用，执行状态更改
	void setAdsorb(vocalshaper::AdsorbState state);
	void setGrid(vocalshaper::GridState state);

private:
	//内部调用，执行状态更改
	void trackChanged(int trackID);
	void totalLengthChanged(double totalLength);
	void currentPositionChanged(double currentTime);
	void followStateChanged(bool followState);
	void loopRangeChanged(double startTime, double endTime);
	void horizontalViewPortChanged(double startTime, double endTime);
	void verticalViewPortChanged(double bottomPitch, double topPitch);

public:
	//被命令调用，设置编辑器状态
	bool isEditMode();
	void setEditMode(bool editMode);
	uint8_t getToolID();
	void setToolID(uint8_t toolID);

public:
	//监听器，用于注册回调监听事件
	void listenTrackSizeChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenProjectLengthChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenAllChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);

	//监听器，用于注册回调监听播放状态
	void listenCurrentPositionChange(double currentTime);
	void listenFollowStateChange(bool followState);
	void listenLoopRangeChange(double startTime, double endTime);

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
	juce::ApplicationCommandManager* commandManager = nullptr;

	void initCommandID();
	void initCommandFunction();
	void initCommandFlagHook();

	void initProjectListener();

	std::unique_ptr<TopEditor> topEditor;
	std::unique_ptr<BottomEditor> bottomEditor;

	std::unique_ptr<juce::StretchableLayoutManager> slManager;
	std::unique_ptr<juce::StretchableLayoutResizerBar> slBar;

	bool trackOpenStateChanged = false;
	bool editModeFlag = false;
	uint8_t toolID = 1;

	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	double totalTimeTemp = 0., totalLengthTemp = 0.;
	juce::ReadWriteLock projectLock;

	static double countProjectTime(vocalshaper::ProjectProxy* ptr);
	static double countTrackTime(vocalshaper::ProjectProxy* ptr, int trackID);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorComponent)
};