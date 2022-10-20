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

		//缓存轨道数和工程长度
		{
			juce::ScopedReadLock projLock(this->project->getLock());
			this->ptrTemp->trackSizeTemp =
				vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
			//TODO 工程长度
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
	}
	else {
		//强制刷新
		this->repaint();
	}
}

void ScrollerBase::setHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		if (this->getVertical()) {
			//TODO 计算值
		}
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setVViewPort(double bottomPer, double topPer)
{

}

void ScrollerBase::setTotalLength(vocalshaper::ProjectTime totalLength)
{
	//TODO 更新滑块大小或强制刷新
}

void ScrollerBase::setCurrentPosition(vocalshaper::ProjectTime currentTime)
{
	//TODO 强制刷新或翻页
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
