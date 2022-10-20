#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ScrollerBase : public vocalshaper::EditorBase
{
public:
	ScrollerBase(bool isVertical = false);
	virtual ~ScrollerBase() override = default;

public:
	bool getVertical();

protected:
	//根据大小限制计算滑块大小
	virtual void limitSize(double& sp, double& ep);
	//绘制背景预览
	virtual void paintPreView(juce::Graphics& g);
	//发送通知
	virtual void noticeChange(double sp, double ep);
	//轨道数量发生改变时重新计算竖直卷滚条位置
	virtual void refreshSizeOnTrackSizeChanged(
		int lastSize, int size, double& sp, double& ep);

public:
	void resized() override;
	void paint(juce::Graphics& g) override;

	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;
	void mouseExit(const juce::MouseEvent& event) override;

public:
	//当项目实例发生切换时被调用
	virtual void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	//当切换当前轨道时被调用
	virtual void trackChanged(int trackID) override;
	//切换水平浏览范围时被调用（针对轨道面板）
	virtual void setHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) override;
	//切换垂直浏览范围时被调用（针对轨道面板）
	virtual void setVViewPort(double bottomPer, double topPer) override;
	//更改总长度时被调用
	virtual void setTotalLength(vocalshaper::ProjectTime totalLength) override;
	//更改当前播放位置时被调用
	virtual void setCurrentPosition(vocalshaper::ProjectTime currentTime) override;

public:
	//监听项目关闭
	void listenProjectClose(const vocalshaper::ProjectProxy* ptr);

private:
	struct Colors final
	{
		juce::Colour background_scroller;
		juce::Colour block_scroller;
	}colors;//界面颜色
	struct Size final
	{
		/*double width_beat_min;
		double width_beat_max;
		double height_track_min;
		double height_track_max;*/
	}sizes;//控件大小
	struct Scales final
	{
	}scales;//控件缩放

	const bool isVertical = false;

	struct SizeTemp
	{
		int trackSizeTemp = 0;			//暂存轨道数量
		uint32_t projectLengthTemp = 0;	//暂存工程长度
		double sp = 0., ep = 1.;		//起止位置
	};
	std::map<const vocalshaper::ProjectProxy*, SizeTemp> tempList;
	SizeTemp* ptrTemp = nullptr;
	juce::ReadWriteLock tempLock;

	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScrollerBase)
};