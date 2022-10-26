#pragma once
#include <JuceHeader.h>
#include "ScrollerBase.h"

class VScroller final : public ScrollerBase
{
public:
	VScroller();
	~VScroller() override = default;

protected:
	//根据大小限制计算滑块大小
	void limitSize(double& sp, double& ep, double nailPer) override;
	//绘制背景预览
	void paintPreView(juce::Graphics& g, int width, int height, bool isVertical) override;
	//发送通知
	void noticeChange(double sp, double ep) override;
	//轨道数量发生改变时重新计算竖直卷滚条位置
	void refreshSizeOnTrackSizeChanged(
		int lastSize, int size,
		std::map<const vocalshaper::Track*, int> trackState, std::map<const vocalshaper::Track*, int> lastTrackState,
		double& sp, double& ep) override;

private:
	struct Size final
	{
		double height_track_min;
		double height_track_max;

		double width_verticalScroller_trackBlock;
		double width_verticalScroller_curveBlock;
		double width_verticalScroller_trackBlockMarginLeft;
		double width_verticalScroller_curveBlockMarginLeft;
	}sizes;//控件大小
	struct Scale final
	{
		double height_curveByTrack;
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VScroller)
};