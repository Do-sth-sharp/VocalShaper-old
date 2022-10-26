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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-verticalScroller-trackBlock", this->sizes.width_verticalScroller_trackBlock, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-verticalScroller-curveBlock", this->sizes.width_verticalScroller_curveBlock, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-verticalScroller-trackBlockMarginLeft", this->sizes.width_verticalScroller_trackBlockMarginLeft, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-verticalScroller-curveBlockMarginLeft", this->sizes.width_verticalScroller_curveBlockMarginLeft, result
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
			double deltaMax = this->getHeight() / (double)minSize;
			double deltaMin = this->getHeight() / (double)maxSize;

			//如果轨道最小状态不能填充完整区域，则以最小状态下的全编辑器高度为完整预览区域，重算delta限制
			if (deltaMax > 1.) {
				deltaMin = deltaMin / deltaMax;
				deltaMax = 1.;
			}

			//根据delta限制滑块大小
			double delta = ep - sp;
			double nail = sp + delta * nailPer;
			if (sp < 0.) { sp = 0.; if (nailPer > 0. && nailPer < 1.) { ep = delta; } }
			if (ep > 1.) { ep = 1.; if (nailPer > 0. && nailPer < 1.) { sp = 1 - delta; } }
			if (delta > deltaMax) { sp = nail - deltaMax * nailPer; ep = sp + deltaMax; }
			if (delta < deltaMin) { sp = nail - deltaMin * nailPer; ep = sp + deltaMin; }
		}
	}
}

void VScroller::paintPreView(juce::Graphics& g, int width, int height)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	juce::ScopedReadLock tempLocker(this->tempLock);
	juce::ScopedReadLock projLocker(this->projectLock);
	if (this->ptrTemp && this->project) {
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
		int trackBlockHeight = height / (trackSize + curveSize * this->scales.height_curveByTrack);
		int curveBlockHeight = trackBlockHeight * this->scales.height_curveByTrack;

		//如果轨道最小状态无法填满全屏，则下方加空间，否则轨道应充满竖直区域
		if (minSize < height) {
			trackBlockHeight = trackMinHeight;
			curveBlockHeight = trackBlockHeight * this->scales.height_curveByTrack;
		}

		//绘制轨道与曲线色块
		{
			juce::ScopedReadLock locker(this->project->getLock());

			//基础大小属性
			int trackBlockXPos = this->sizes.width_verticalScroller_trackBlockMarginLeft * screenSize.getWidth();
			int curveBlockXPos = this->sizes.width_verticalScroller_curveBlockMarginLeft * screenSize.getWidth();
			int width_trackBlock = this->sizes.width_verticalScroller_trackBlock * screenSize.getWidth();
			int width_curveBlock = this->sizes.width_verticalScroller_curveBlock * screenSize.getWidth();
			int YPos = 0;

			//绘主轨
			{
				auto track = vocalshaper::ProjectDAO::getMasterTrack(this->project->getPtr());
				
				g.setColour(vocalshaper::TrackDAO::getColour(track));

				g.fillRect(trackBlockXPos, YPos, width_trackBlock, trackBlockHeight);
				YPos += trackBlockHeight;

				auto it = this->ptrTemp->trackState.find(track);
				if (it != this->ptrTemp->trackState.end()) {
					if (it->second > 0) {
						g.fillRect(curveBlockXPos, YPos, width_curveBlock, curveBlockHeight * it->second);
						YPos += (curveBlockHeight * it->second);
					}
				}
			}

			//绘其它轨
			for (int i = 0; i < vocalshaper::ProjectDAO::trackSize(this->project->getPtr()); i++) {
				auto track = vocalshaper::ProjectDAO::getTrack(this->project->getPtr(), i);

				g.setColour(vocalshaper::TrackDAO::getColour(track));

				g.fillRect(trackBlockXPos, YPos, width_trackBlock, trackBlockHeight);
				YPos += trackBlockHeight;

				auto it = this->ptrTemp->trackState.find(track);
				if (it != this->ptrTemp->trackState.end()) {
					if (it->second > 0) {
						g.fillRect(curveBlockXPos, YPos, width_curveBlock, curveBlockHeight * it->second);
						YPos += (curveBlockHeight * it->second);
					}
				}
			}
		}
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
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算轨道区最大最小大小
	int trackMaxHeight = this->sizes.height_track_max * screenSize.getHeight();
	int trackMinHeight = this->sizes.height_track_min * screenSize.getHeight();
	int curveMaxHeight = trackMaxHeight * this->scales.height_curveByTrack;
	int curveMinHeight = trackMinHeight * this->scales.height_curveByTrack;

	int trackSize = size + 1;
	int curveSize = 0;
	for (auto& i : trackState) {
		curveSize += i.second;
	}
	int lastTrackSize = lastSize + 1;
	int lastCurveSize = 0;
	for (auto& i : lastTrackState) {
		lastCurveSize += i.second;
	}

	int maxSize = trackSize * trackMaxHeight + curveSize * curveMaxHeight;
	int minSize = trackSize * trackMinHeight + curveSize * curveMinHeight;
	int lastMaxSize = lastTrackSize * trackMaxHeight + lastCurveSize * curveMaxHeight;
	int lastMinSize = lastTrackSize * trackMinHeight + lastCurveSize * curveMinHeight;

	//计算原先状态和新状态是否有多余空间
	bool overFlowMode = (minSize < this->getHeight());
	bool lastOverFlowMode = (lastMinSize < this->getHeight());

	//计算原位置
	double SP = sp * (lastTrackSize + lastCurveSize * this->scales.height_curveByTrack);
	double EP = ep * (lastTrackSize + lastCurveSize * this->scales.height_curveByTrack);
	if (lastOverFlowMode) {
		SP = (sp / (lastMinSize / (double)this->getHeight())) * (lastTrackSize + lastCurveSize * this->scales.height_curveByTrack);
		EP = (ep / (lastMinSize / (double)this->getHeight())) * (lastTrackSize + lastCurveSize * this->scales.height_curveByTrack);
	}

	//计算新位置
	sp = SP / (trackSize + curveSize * this->scales.height_curveByTrack);
	ep = EP / (trackSize + curveSize * this->scales.height_curveByTrack);
	if (overFlowMode) {
		sp = (SP / (trackSize + curveSize * this->scales.height_curveByTrack)) * (minSize / (double)this->getHeight());
		ep = (EP / (trackSize + curveSize * this->scales.height_curveByTrack)) * (minSize / (double)this->getHeight());
	}
}

void VScroller::refreshSizeOnResized(int lastSize, int size, double& sp, double& ep)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	juce::ScopedReadLock tempLocker(this->tempLock);
	juce::ScopedReadLock projLocker(this->projectLock);
	if (this->ptrTemp && this->project) {
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

		//计算原先状态和新状态是否有多余空间
		bool overFlowMode = (minSize < size);
		bool lastOverFlowMode = (minSize < lastSize);

		//计算原位置
		double SP = sp * (trackSize + curveSize * this->scales.height_curveByTrack);
		double EP = ep * (trackSize + curveSize * this->scales.height_curveByTrack);
		if (lastOverFlowMode) {
			SP = (sp / (minSize / (double)lastSize)) * (trackSize + curveSize * this->scales.height_curveByTrack);
			EP = (ep / (minSize / (double)lastSize)) * (trackSize + curveSize * this->scales.height_curveByTrack);
		}

		//控制缩放
		double ratio = (EP - SP) / lastSize;
		EP = SP + ratio * size;

		//计算新位置
		sp = SP / (trackSize + curveSize * this->scales.height_curveByTrack);
		ep = EP / (trackSize + curveSize * this->scales.height_curveByTrack);
		if (overFlowMode) {
			sp = (SP / (trackSize + curveSize * this->scales.height_curveByTrack)) * (minSize / (double)size);
			ep = (EP / (trackSize + curveSize * this->scales.height_curveByTrack)) * (minSize / (double)size);
		}
	}
}
