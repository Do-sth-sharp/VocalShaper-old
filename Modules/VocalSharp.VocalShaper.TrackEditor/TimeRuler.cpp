#include "TimeRuler.h"
#include <libJModule.h>

TimeRuler::TimeRuler()
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
		"main", "color", "background-timeRuler", this->colors.background_timeRuler, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border", this->colors.border, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-borderTop", this->sizes.height_borderTop, result
		);

	//position
	//scale

	//resource
}

void TimeRuler::resized()
{

}

void TimeRuler::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//填充背景
	g.fillAll(this->colors.background_timeRuler);

	//计算控件大小
	int height_borderTop = this->sizes.height_borderTop * screenSize.getHeight();

	//绘制标尺上边框
	juce::Rectangle<int> rectRulerHeadTopBorder(
		0, 0,
		this->getWidth(), height_borderTop
	);
	g.setColour(this->colors.border);
	g.fillRect(rectRulerHeadTopBorder);
}

void TimeRuler::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);
	//TODO 刷新
}

void TimeRuler::setEditMode(bool editMode)
{
	this->editModeFlag = editMode;
}

void TimeRuler::setToolID(uint8_t toolID)
{
	this->toolID = toolID;
}

void TimeRuler::trackChanged(int trackID)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->trackID = trackID;
	//TODO 刷新
}

void TimeRuler::setTotalLength(vocalshaper::ProjectTime totalLength)
{
	//TODO
}

void TimeRuler::setCurrentPosition(vocalshaper::ProjectTime currentTime)
{
	//TODO
}

void TimeRuler::setAdsorb(vocalshaper::AdsorbState state)
{
	//TODO
}

void TimeRuler::setGrid(vocalshaper::GridState state)
{
	//TODO
}
