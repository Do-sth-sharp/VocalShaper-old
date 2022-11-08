#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackList final : public vocalshaper::EditorBase
{
public:
	TrackList();

	void resized() override;
	void paint(juce::Graphics& g) override;

	void setMethods(
		const std::function<void(int)>& setCurrentTrackFunc,
		const std::function<void(void)>& refreshTotalTimeFunc,
		const std::function<void(double)>& setCurrentPositionFunc,
		const std::function<void(double, double)>& setLoopRangeFunc,
		const std::function<void(double, double)>& setHorizontalViewPortFunc,
		const std::function<void(double, double)>& setVerticalViewPortFunc
	) override;
	void setTrackViewMethods(
		std::function<void(double, double)> setHViewPortFunc,
		std::function<void(double, double)> setVViewPortFunc
	) override;

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
	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;

	double startTime = 0., endTime = 0.;
	double bottomTrack = 0., topTrack = 0.;
	double loopStartTime = 0., loopEndTime = 0.;
	double currentTime = 0.;

	vocalshaper::AdsorbState adsorb = vocalshaper::AdsorbState::Adsorb1Beat;
	vocalshaper::GridState grid = vocalshaper::GridState::Grid1Beat;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackList)
};