#include "ScrollerBase.h"

ScrollerBase::ScrollerBase(bool isVertical)
	: EditorBase(), isVertical(isVertical)
{

}

bool ScrollerBase::getVertical()
{
	return this->isVertical;
}

void ScrollerBase::limitSize(double& sp, double& ep)
{
	if (sp < 0.) { sp = 0.; }
	if (ep > 1.) { ep = 1.; }
	if (ep < sp) { std::swap(ep, sp); }
}

void ScrollerBase::paintPreView(juce::Graphics& g)
{
}

void ScrollerBase::noticeChange(double sp, double ep)
{
}

void ScrollerBase::refreshSizeOnTrackSizeChanged(
	int lastSize, int size, double& sp, double& ep)
{
}

void ScrollerBase::refreshSizeOnProjectLengthChanged(
	uint32_t lastLength, uint32_t length, double& sp, double& ep)
{
}

//
void ScrollerBase::resized()
{

}

void ScrollerBase::paint(juce::Graphics& g)
{

}

//
void ScrollerBase::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details)
{

}

void ScrollerBase::mouseDown(const juce::MouseEvent& event)
{

}

void ScrollerBase::mouseDrag(const juce::MouseEvent& event)
{

}

void ScrollerBase::mouseUp(const juce::MouseEvent& event)
{

}

void ScrollerBase::mouseExit(const juce::MouseEvent& event)
{

}

//
void ScrollerBase::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker1(this->tempLock),locker2(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);

	if (ptr) {
		//获取或建立缓存
		auto it = this->tempList.find(ptr);
		if (it != this->tempList.end()) {
			this->ptrTemp = &(it->second);
		}
		else {
			this->ptrTemp = &(this->tempList[ptr]);
		}

		//缓存工程相关信息
		{
			juce::ScopedReadLock projLock(this->project->getLock());

			this->ptrTemp->followTemp = this->isFollow;

			this->ptrTemp->trackSizeTemp =
				vocalshaper::ProjectDAO::trackSize(this->project->getPtr());

			this->ptrTemp->projectCurveQuantTemp =
				vocalshaper::ProjectDAO::getCurveQuantification(this->project->getPtr());

			vocalshaper::ProjectTime totalLength =
				vocalshaper::CountTime::count(
					this->project->getPtr(), this->ptrTemp->projectCurveQuantTemp);
			uint32_t bar =
				this->project->getBeat()->getBarAtTime(
					vocalshaper::floor(totalLength, this->ptrTemp->projectCurveQuantTemp).first);
			bar = std::max(bar, 20Ui32) + 1;
			this->ptrTemp->projectLengthTemp =
				this->project->getBeat()->getTimeAtBar(bar);
		}

		//限制大小
		this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

		//发送改变
		this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
	}
	else {
		this->ptrTemp = nullptr;
		this->repaint();
	}
}

void ScrollerBase::trackChanged(int trackID)
{
	juce::ScopedWriteLock locker1(this->tempLock), locker2(this->projectLock);
	this->trackID = trackID;
	if (!this->project || !this->ptrTemp) {
		//刷新
		this->repaint();
		return;
	}

	//获取轨道数
	int trackSize = this->ptrTemp->trackSizeTemp;
	{
		juce::ScopedReadLock projLocker(this->project->getLock());
		trackSize = vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
	}

	if ((trackSize != this->ptrTemp->trackSizeTemp) && this->getVertical()) {
		//计算新缩放
		this->refreshSizeOnTrackSizeChanged(this->ptrTemp->trackSizeTemp, trackSize,
			this->ptrTemp->sp, this->ptrTemp->ep);

		//更新缓存
		this->ptrTemp->trackSizeTemp = trackSize;

		//限制大小
		this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

		//发送改变
		this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

		return;
	}
	
	//更新缓存
	this->ptrTemp->trackSizeTemp = trackSize;

	//刷新
	this->repaint();
}

void ScrollerBase::setHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		if (!this->getVertical()) {
			//计算起止位置
			this->ptrTemp->sp =
				vocalshaper::timeToDouble(startTime, this->ptrTemp->projectCurveQuantTemp)
				/ this->ptrTemp->projectLengthTemp;
			this->ptrTemp->ep =
				vocalshaper::timeToDouble(endTime, this->ptrTemp->projectCurveQuantTemp)
				/ this->ptrTemp->projectLengthTemp;
		}
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setVViewPort(double bottomPer, double topPer)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		if (this->getVertical()) {
			//计算起止位置
			this->ptrTemp->sp = topPer;
			this->ptrTemp->ep = bottomPer;
		}
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setTotalLength(vocalshaper::ProjectTime totalLength)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		//取缓存
		auto trackSizeTemp = this->ptrTemp->trackSizeTemp;
		auto projectCurveQuantTemp = this->ptrTemp->projectCurveQuantTemp;
		auto projectLengthTemp = this->ptrTemp->projectLengthTemp;

		//计算工程相关信息
		{
			juce::ScopedReadLock projLock(this->project->getLock());

			trackSizeTemp =
				vocalshaper::ProjectDAO::trackSize(this->project->getPtr());

			projectCurveQuantTemp =
				vocalshaper::ProjectDAO::getCurveQuantification(this->project->getPtr());

			vocalshaper::ProjectTime totalLength =
				vocalshaper::CountTime::count(
					this->project->getPtr(), projectCurveQuantTemp);
			uint32_t bar =
				this->project->getBeat()->getBarAtTime(
					vocalshaper::floor(totalLength, projectCurveQuantTemp).first);
			bar = std::max(bar, 20Ui32) + 1;
			projectLengthTemp =
				this->project->getBeat()->getTimeAtBar(bar);
		}

		//如果是水平，则更新滑块大小
		if (!this->getVertical()) {
			//计算新缩放
			this->refreshSizeOnProjectLengthChanged(
				this->ptrTemp->projectLengthTemp, projectLengthTemp,
				this->ptrTemp->sp, this->ptrTemp->ep);

			//更新缓存
			this->ptrTemp->trackSizeTemp = trackSizeTemp;
			this->ptrTemp->projectCurveQuantTemp = projectCurveQuantTemp;
			this->ptrTemp->projectLengthTemp = projectLengthTemp;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			return;
		}

		//更新缓存
		this->ptrTemp->trackSizeTemp = trackSizeTemp;
		this->ptrTemp->projectCurveQuantTemp = projectCurveQuantTemp;
		this->ptrTemp->projectLengthTemp = projectLengthTemp;
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setCurrentPosition(vocalshaper::ProjectTime currentTime)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		//更新缓存
		this->ptrTemp->currentPositionTemp =
			vocalshaper::timeToU64(currentTime, this->ptrTemp->projectCurveQuantTemp);

		if (!this->getVertical()) {
			//翻页
			if (this->ptrTemp->followTemp) {
				//计算当前百分比
				double per = this->ptrTemp->currentPositionTemp / (double)this->ptrTemp->projectLengthTemp;

				//需要翻页
				if (per < this->ptrTemp->sp || per >= this->ptrTemp->ep) {
					//计算翻页位置
					double diff = this->ptrTemp->ep - this->ptrTemp->sp;
					this->ptrTemp->sp = per;
					this->ptrTemp->ep = per + diff;

					//限制大小
					this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

					//发送改变
					this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
				}
			}

			//刷新
			this->repaint();
		}
	}
}

void ScrollerBase::setFollowState(bool follow)
{
	this->isFollow = follow;
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		this->ptrTemp->followTemp = follow;
	}
}

void ScrollerBase::listenProjectClose(const vocalshaper::ProjectProxy* ptr)
{
	//清除对应缓存
	juce::ScopedWriteLock locker(this->tempLock);
	auto it = this->tempList.find(ptr);
	if (it != this->tempList.end()) {
		if (this->ptrTemp == &(it->second)) {
			this->ptrTemp = nullptr;
		}
		this->tempList.erase(it);
	}
	this->repaint();
}
