#include "TrackEditor.h"
#include <libJModule.h>

TrackEditor::TrackEditor()
	:EditorBase()
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//以下获取界面属性
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background", this->colors.background, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHead", this->sizes.width_trackHead, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeRuler", this->sizes.height_timeRuler, result
		);

	//position
	//scale
	
	//resource
}

void TrackEditor::resized()
{

}

void TrackEditor::paint(juce::Graphics& g)
{
	g.fillAll(this->colors.background);
}

void TrackEditor::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);
	//TODO 刷新
}

void TrackEditor::setEditMode(bool editMode)
{
	this->editModeFlag = editMode;
}

void TrackEditor::setToolID(uint8_t toolID)
{
	this->toolID = toolID;
}

void TrackEditor::trackChanged(int trackID)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->trackID = trackID;
	//TODO 刷新
}

void TrackEditor::setTotalLength(vocalshaper::ProjectTime totalLength)
{
	//TODO
}

void TrackEditor::setCurrentPosition(vocalshaper::ProjectTime currentTime)
{
	//TODO
}