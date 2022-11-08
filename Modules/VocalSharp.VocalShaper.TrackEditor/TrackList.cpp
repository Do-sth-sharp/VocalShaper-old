#include "TrackList.h"

TrackList::TrackList()
	: EditorBase()
{
	this->setOpaque(false);
}

void TrackList::resized()
{

}

void TrackList::paint(juce::Graphics& g)
{

}

void TrackList::setMethods(
	const std::function<void(int)>& setCurrentTrackFunc,
	const std::function<void(void)>& refreshTotalTimeFunc,
	const std::function<void(double)>& setCurrentPositionFunc,
	const std::function<void(double, double)>& setLoopRangeFunc,
	const std::function<void(double, double)>& setHorizontalViewPortFunc,
	const std::function<void(double, double)>& setVerticalViewPortFunc
)
{
	//TODO
	this->EditorBase::setMethods(
		setCurrentTrackFunc, refreshTotalTimeFunc, setCurrentPositionFunc,
		setLoopRangeFunc, setHorizontalViewPortFunc, setVerticalViewPortFunc);
}

void TrackList::setTrackViewMethods(
	std::function<void(double, double)> setHViewPortFunc,
	std::function<void(double, double)> setVViewPortFunc
)
{
	//TODO
	this->EditorBase::setTrackViewMethods(setHViewPortFunc, setVViewPortFunc);
}

void TrackList::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);
	//TODO
}

void TrackList::setEditMode(bool editMode)
{
	this->editModeFlag = editMode;
	//TODO
}

void TrackList::setToolID(uint8_t toolID)
{
	this->toolID = toolID;
	//TODO
}

void TrackList::trackChanged(int trackID)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->trackID = trackID;
	//TODO
}

void TrackList::setTotalLength(double totalLength)
{
	//TODO
}

void TrackList::setCurrentPosition(double currentTime)
{
	this->currentTime = currentTime;
	//TODO
}

void TrackList::setLoopRange(double startTime, double endTime)
{
	this->loopStartTime = startTime;
	this->loopEndTime = endTime;
	//TODO
}

void TrackList::setHorizontalViewPort(double startTime, double endTime)
{
	//TODO
}

void TrackList::setVerticalViewPort(double bottomPitch, double topPitch)
{
	//TODO
}

void TrackList::setHViewPort(double startTime, double endTime)
{
	this->startTime = startTime;
	this->endTime = endTime;
	//TODO
}

void TrackList::setVViewPort(double bottomTrack, double topTrack)
{
	this->bottomTrack = bottomTrack;
	this->topTrack = topTrack;
	//TODO
}

void TrackList::setAdsorb(vocalshaper::AdsorbState state)
{
	this->adsorb = state;
	//TODO
}

void TrackList::setGrid(vocalshaper::GridState state)
{
	this->grid = state;
	//TODO
}
