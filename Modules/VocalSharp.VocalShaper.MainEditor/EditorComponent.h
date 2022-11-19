#pragma once
#include <JuceHeader.h>
#include "TopEditor.h"
#include "BottomEditor.h"
#include <libVocalShaper.h>

class EditorComponent final : public vocalshaper::EditorBase
{
public:
	EditorComponent();
	~EditorComponent() override;

public:
	//被命令调用，设置编辑器显示状态
	void setTrackOpen(bool trackOpen);
	bool isTrackOpen();

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
	void wannaSetCurrentTrack(int trackID);
	void wannaRefreshTotalLength();
	void wannaSetCurrentPosition(double currentTime);
	void wannaSetLoopRange(double startTime, double endTime);
	void wannaSetHorizontalViewPort(double startTime, double endTime);
	void wannaSetVerticalViewPort(double bottomPitch, double topPitch);
	void wannaChangeHViewPort(double startTime, double endTime);
	void wannaChangeVViewPort(double bottomTrack, double topTrack);

protected:
	//内部调用，状态更改回调
	void setProjectCallback(const vocalshaper::ProjectProxy* ptr) override;

public:
	//监听器，用于注册回调监听事件
	void listenTrackSizeChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenProjectLengthChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenAllChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);

	//监听器，用于注册回调监听播放状态
	void listenCurrentPositionChange(double currentTime);
	void listenFollowStateChange(bool followState);
	void listenLoopRangeChange(double startTime, double endTime);

	//监听器，监听项目列表
	void listenProjectClose(const vocalshaper::ProjectProxy* ptr);

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
		std::unique_ptr<juce::LookAndFeel> stretchableBar;
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
	double totalTimeTemp = 0., totalLengthTemp = 0.;

	static double countProjectTime(vocalshaper::ProjectProxy* ptr);
	static double countTrackTime(vocalshaper::ProjectProxy* ptr, int trackID);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorComponent)
};