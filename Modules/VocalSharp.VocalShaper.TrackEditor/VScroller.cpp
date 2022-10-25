#include "VScroller.h"
#include <libJModule.h>

VScroller::VScroller()
	: ScrollerBase(true)
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//以下获取界面属性
	bool result = false;
	//color
	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-track-min", this->sizes.height_track_min, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-track-max", this->sizes.height_track_max, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-curveByTrack", this->scales.height_curveByTrack, result
		);

	//resource
}

void VScroller::limitSize(double& sp, double& ep, double nailPer)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//进行基础限制
	this->ScrollerBase::limitSize(sp, ep, nailPer);
	
	{
		juce::ScopedReadLock tempLocker(this->tempLock);
		if (this->ptrTemp) {
			//计算轨道区最大最小大小
			int trackMaxHeight = this->sizes.height_track_max * screenSize.getHeight();
			int trackMinHeight = this->sizes.height_track_min * screenSize.getHeight();
			int curveMaxHeight = trackMaxHeight * this->scales.height_curveByTrack;
			int curveMinHeight = trackMinHeight * this->scales.height_curveByTrack;

			int trackSize = this->ptrTemp->trackSizeTemp + 1;
			int curveSize = 0;
			for (auto& i : this->ptrTemp->trackState) {
				curveSize += i.second;
			}

			int maxSize = trackSize * trackMaxHeight + curveSize * curveMaxHeight;
			int minSize = trackSize * trackMinHeight + curveSize * curveMinHeight;

			//计算最大最小delta
			double deltaMax = minSize / (double)this->getHeight();
			double deltaMin = maxSize / (double)this->getHeight();

			//如果轨道最小状态不能填充完整区域，则以最小状态下的全编辑器高度为完整预览区域，重算delta限制
			if (deltaMax > 1.) {
				deltaMin = deltaMin / deltaMax;
				deltaMax = 1.;
			}

			//根据delta限制滑块大小
			double delta = ep - sp;
			double nail = sp + delta * nailPer;
			if (delta > deltaMax) { sp = nail - deltaMax * nailPer; ep = sp + deltaMax; }
			if (delta < deltaMin) { sp = nail - deltaMin * nailPer; ep = sp + deltaMin; }
		}
	}
}

void VScroller::paintPreView(juce::Graphics& g, int width, int height, bool /*isVertical*/)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	juce::ScopedReadLock tempLocker(this->tempLock);
	if (this->ptrTemp) {
		//计算轨道区最大最小大小
		int trackMaxHeight = this->sizes.height_track_max * screenSize.getHeight();
		int trackMinHeight = this->sizes.height_track_min * screenSize.getHeight();
		int curveMaxHeight = trackMaxHeight * this->scales.height_curveByTrack;
		int curveMinHeight = trackMinHeight * this->scales.height_curveByTrack;

		int trackSize = this->ptrTemp->trackSizeTemp + 1;
		int curveSize = 0;
		for (auto& i : this->ptrTemp->trackState) {
			curveSize += i.second;
		}

		int maxSize = trackSize * trackMaxHeight + curveSize * curveMaxHeight;
		int minSize = trackSize * trackMinHeight + curveSize * curveMinHeight;

		//计算当前轨道色块大小
		int trackBlockHeight = this->getHeight() / (trackSize + curveSize * this->scales.height_curveByTrack);
		int curveBlockHeight = trackBlockHeight * this->scales.height_curveByTrack;

		//如果轨道最小状态无法填满全屏，则下方加空间，否则轨道应充满竖直区域
		if (minSize < this->getHeight()) {
			trackBlockHeight = (trackMinHeight / (double)minSize) * this->getHeight();
			curveBlockHeight = trackBlockHeight * this->scales.height_curveByTrack;
		}

		//TODO 绘制色块
	}
}

void VScroller::noticeChange(double sp, double ep)
{
	this->setVViewPortMethod(ep, sp);
}

void VScroller::refreshSizeOnTrackSizeChanged(
	int lastSize, int size,
	std::map<const vocalshaper::Track*, int> trackState, std::map<const vocalshaper::Track*, int> lastTrackState, 
	double& sp, double& ep)
{
	//TODO 维持原缩放和起点为目的（要分别判断是否填充），调整预览区域
}