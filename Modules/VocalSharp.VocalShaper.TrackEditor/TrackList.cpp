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

void TrackList::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	this->EditorBase::projectChanged(ptr);
	//TODO
}

void TrackList::setEditMode(bool editMode)
{
	this->EditorBase::setEditMode(editMode);
	//TODO
}

void TrackList::setToolID(uint8_t toolID)
{
	this->EditorBase::setToolID(toolID);
	//TODO
}

void TrackList::trackChanged(int trackID)
{
	this->EditorBase::trackChanged(trackID);
	//TODO
}

void TrackList::setTotalLength(double totalLength)
{
	this->EditorBase::setTotalLength(totalLength);
	//TODO
}

void TrackList::setCurrentPosition(double currentTime)
{
	this->EditorBase::setCurrentPosition(currentTime);
	//TODO
}

void TrackList::setLoopRange(double startTime, double endTime)
{
	this->EditorBase::setLoopRange(startTime, endTime);
	//TODO
}

void TrackList::setHorizontalViewPort(double startTime, double endTime)
{
	this->EditorBase::setHorizontalViewPort(startTime, endTime);
	//TODO
}

void TrackList::setVerticalViewPort(double bottomPitch, double topPitch)
{
	this->EditorBase::setVerticalViewPort(bottomPitch, topPitch);
	//TODO
}

void TrackList::setHViewPort(double startTime, double endTime)
{
	this->EditorBase::setHViewPort(startTime, endTime);
	//TODO
}

void TrackList::setVViewPort(double bottomTrack, double topTrack)
{
	this->EditorBase::setVViewPort(bottomTrack, topTrack);
	//TODO
}

void TrackList::setAdsorb(vocalshaper::AdsorbState state)
{
	this->EditorBase::setAdsorb(state);
	//TODO
}

void TrackList::setGrid(vocalshaper::GridState state)
{
	this->EditorBase::setGrid(state);
	//TODO
}
