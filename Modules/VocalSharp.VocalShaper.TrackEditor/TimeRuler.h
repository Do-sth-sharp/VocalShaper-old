#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TimeRuler final : public vocalshaper::EditorBase
{
public:
	TimeRuler();
	~TimeRuler() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

public:
	void projectChanged(const vocalshaper::ProjectProxy* ptr) override;
	void setEditMode(bool editMode) override;
	void setToolID(uint8_t toolID) override;
	void trackChanged(int trackID) override;
	void setTotalLength(vocalshaper::ProjectTime totalLength) override;
	void setCurrentPosition(vocalshaper::ProjectTime currentTime) override;
	void setAdsorb(vocalshaper::AdsorbState state) override;
	void setGrid(vocalshaper::GridState state) override;
	void setHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) override;
	void setVViewPort(double bottomPer, double topPer) override;

private:
	struct Colors final
	{
		juce::Colour background_timeRuler;

		juce::Colour border;
	}colors;//界面颜色
	struct Size final
	{
		double height_borderTop;
	}sizes;//控件大小
	struct Scales final
	{
	}scales;//控件缩放

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;

	vocalshaper::ProjectProxy* project = nullptr;
	int trackID = -1;
	juce::ReadWriteLock projectLock;

	bool editModeFlag = false;
	uint8_t toolID = 1;
};