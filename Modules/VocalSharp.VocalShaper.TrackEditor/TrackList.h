#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackList final : public vocalshaper::EditorBase
{
public:
	TrackList();

	void resized() override;
	void paint(juce::Graphics& g) override;

public:
	void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	void setEditMode(bool editMode) override;
	void setToolID(uint8_t toolID) override;
	void trackChanged(int trackID) override;
	void setTotalLength(double totalLength) override;
	void setCurrentPosition(double currentTime) override;
	void setLoopRange(double startTime, double endTime) override;
	void setHorizontalViewPort(double startTime, double endTime) override;
	void setVerticalViewPort(double bottomPitch, double topPitch) override;
	void setHViewPort(double startTime, double endTime) override;
	void setVViewPort(double bottomTrack, double topTrack) override;
	void setAdsorb(vocalshaper::AdsorbState state) override;
	void setGrid(vocalshaper::GridState state) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackList)
};