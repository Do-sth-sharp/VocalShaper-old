#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ScrollerBase : public vocalshaper::EditorBase
{
public:
	ScrollerBase(bool isVertical = false);
	virtual ~ScrollerBase() override = default;

public:
	bool getVertical() const;
	void showCurve(const vocalshaper::Track* track, bool show);

	void sendWheelChange(double per, double delta);
	void sendWheelChangeWithCtrl(double per, double delta);

protected:
	//根据大小限制计算滑块大小
	virtual void limitSize(double& sp, double& ep, double nailPer);
	//绘制背景预览
	virtual void paintPreView(juce::Graphics& g, int width, int height);
	//发送通知
	virtual void noticeChange(double sp, double ep);
	//轨道数量和展开状态发生改变时重新计算竖直卷滚条位置
	virtual void refreshSizeOnTrackSizeChanged(
		int lastSize, int size,
		std::map<const vocalshaper::Track*, int> trackState, std::map<const vocalshaper::Track*, int> lastTrackState,
		double& sp, double& ep);
	//工程长度发生改变时重新计算水平卷滚条位置
	virtual void refreshSizeOnProjectLengthChanged(
		double lastLength, double length, double& sp, double& ep);
	//控件大小发生改变时重算卷滚条位置
	virtual void refreshSizeOnResized(int lastSize, int size, double& sp, double& ep);
	//从轨道位置计算卷滚条百分比
	virtual void updateVViewPort(double bottomTrack, double topTrack, double& sp, double& ep);

public:
	void resized() override;
	void paint(juce::Graphics& g) override;

	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseMove(const juce::MouseEvent& event) override;
	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;
	void mouseExit(const juce::MouseEvent& event) override;

public:
	//当项目实例发生切换时被调用
	virtual void projectChangedCallback(const vocalshaper::ProjectProxy* ptr) override;
	//当切换当前轨道时被调用
	virtual void trackChangedCallback(int trackID) override;
	//切换水平浏览范围时被调用（针对轨道面板）
	virtual void setHViewPortCallback(double startTime, double endTime) override;
	//切换垂直浏览范围时被调用（针对轨道面板）
	virtual void setVViewPortCallback(double bottomTrack, double topTrack) override;
	//更改总长度时被调用
	virtual void setTotalLengthCallback(double totalLength) override;
	//更改当前播放位置时被调用
	virtual void setCurrentPositionCallback(double currentTime) override;
	//更改播放跟随状态时被调用
	virtual void setFollowStateCallback(bool follow) override;
	//更改播放循环范围时被调用
	virtual void setLoopRangeCallback(double startTime, double endTime) override;

public:
	//监听项目关闭
	void listenProjectClose(const vocalshaper::ProjectProxy* ptr);

	//监听曲线数量变化
	void listenCurveChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);

protected:

	struct SizeTemp
	{
		int trackSizeTemp = 0;									//暂存轨道数量
		double projectLengthTemp = 0;							//暂存工程长度
		double currentPositionTemp = 0;							//暂存当前播放位置
		bool followTemp = true;									//暂存跟随状态
		double sp = 0., ep = 1.;								//起止位置
		double loopST = 0., loopET = 0.;						//播放选区起止
		std::map<const vocalshaper::Track*, int> trackState;	//记录轨道展开状态
	};
	std::map<const vocalshaper::ProjectProxy*, SizeTemp> tempList;
	SizeTemp* ptrTemp = nullptr;
	juce::ReadWriteLock tempLock;

private:
	struct Colors final
	{
		juce::Colour background_scroller;
		juce::Colour block_scroller;
	}colors;//界面颜色
	struct Size final
	{
		double width_scrollerBlockJudgeArea;
		double height_scrollerBlockJudgeArea;
		double width_scrollerMargin;
		double height_scrollerMargin;
		double width_scrollerBlockRadix;
		double height_scrollerBlockRadix;
	}sizes;//控件大小
	struct Scales final
	{
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	const bool isVertical = false;

	enum class ScrollerState {
		Normal,					//正常状态
		SPChange,				//前端位置更改
		EPChange,				//后端位置更改
		BlockChange				//滑块位置更改
	}scrollerState = ScrollerState::Normal;			//当前状态
	bool scrollerBlockHighlight = false;			//滑块高亮
	bool scrollerBlockBorderHighlight = false;		//滑块边界高亮
	double spTemp = 0., epTemp = 1.;				//按下时状态缓存
	double blockPerTemp = 0.5;						//按在滑块的位置百分比

	int sizeTemp = 0;			//卷滚条长度缓存

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScrollerBase)
};