#pragma once
#include <JuceHeader.h>
#include "ScrollerBase.h"

class HScroller final : public ScrollerBase
{
public:
	HScroller();
	~HScroller() override = default;

protected:
	//根据大小限制计算滑块大小
	void limitSize(double& sp, double& ep, double nailPer) override;
	//绘制背景预览
	void paintPreView(juce::Graphics& g, int width, int height) override;
	//发送通知
	void noticeChange(double sp, double ep) override;
	//工程长度发生改变时重新计算水平卷滚条位置
	void refreshSizeOnProjectLengthChanged(
		double lastLength, double length, double& sp, double& ep) override;
	//控件大小发生改变时重算卷滚条位置
	void refreshSizeOnResized(int lastSize, int size, double& sp, double& ep) override;

private:
	struct Colors final
	{
		juce::Colour cursor;
	}colors;//界面颜色
	struct Size final
	{
		double width_beat_min;
		double width_beat_max;

		double width_cursor;
	}sizes;//控件大小

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HScroller)
};