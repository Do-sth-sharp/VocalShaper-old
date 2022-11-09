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

void TrackList::projectChangedCallback(const vocalshaper::ProjectProxy* ptr)
{
	//TODO
}

void TrackList::setEditModeCallback(bool editMode)
{
	//TODO
}

void TrackList::setToolIDCallback(uint8_t toolID)
{
	//TODO
}

void TrackList::trackChangedCallback(int trackID)
{
	//TODO
}

void TrackList::setTotalLengthCallback(double totalLength)
{
	//TODO
}

void TrackList::setCurrentPositionCallback(double currentTime)
{
	//TODO
}

void TrackList::setLoopRangeCallback(double startTime, double endTime)
{
	//TODO
}

void TrackList::setHorizontalViewPortCallback(double startTime, double endTime)
{
	//TODO
}

void TrackList::setVerticalViewPortCallback(double bottomPitch, double topPitch)
{
	//TODO
}

void TrackList::setHViewPortCallback(double startTime, double endTime)
{
	//TODO
}

void TrackList::setVViewPortCallback(double bottomTrack, double topTrack)
{
	//TODO
}

void TrackList::setAdsorbCallback(vocalshaper::AdsorbState state)
{
	//TODO
}

void TrackList::setGridCallback(vocalshaper::GridState state)
{
	//TODO
}
