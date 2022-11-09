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
	void projectChangedCallback(const vocalshaper::ProjectProxy* ptr) override;
	void setEditModeCallback(bool editMode) override;
	void setToolIDCallback(uint8_t toolID) override;
	void trackChangedCallback(int trackID) override;
	void setTotalLengthCallback(double totalLength) override;
	void setCurrentPositionCallback(double currentTime) override;
	void setLoopRangeCallback(double startTime, double endTime) override;
	void setHorizontalViewPortCallback(double startTime, double endTime) override;
	void setVerticalViewPortCallback(double bottomPitch, double topPitch) override;
	void setHViewPortCallback(double startTime, double endTime) override;
	void setVViewPortCallback(double bottomTrack, double topTrack) override;
	void setAdsorbCallback(vocalshaper::AdsorbState state) override;
	void setGridCallback(vocalshaper::GridState state) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackList)
};