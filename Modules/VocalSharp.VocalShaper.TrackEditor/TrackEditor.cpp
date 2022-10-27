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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border", this->colors.border, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-rulerHead", this->colors.background_rulerHead, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-timeRuler", this->colors.background_timeRuler, result
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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-verticalScroller", this->sizes.width_verticalScroller, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-horizontalScroller", this->sizes.height_horizontalScroller, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-borderTop", this->sizes.height_borderTop, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-borderRight", this->sizes.width_borderRight, result
		);

	//position
	//scale
	
	//resource

	//建立时间标尺
	this->timeRuler = std::make_unique<TimeRuler>();
	this->addAndMakeVisible(this->timeRuler.get());

	//建立竖直卷滚条
	this->vScroller = std::make_unique<VScroller>();
	this->addAndMakeVisible(this->vScroller.get());

	//建立水平卷滚条
	this->hScroller = std::make_unique<HScroller>();
	this->addAndMakeVisible(this->hScroller.get());
}

void TrackEditor::resized()
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int width_trackHead = this->sizes.width_trackHead * screenSize.getWidth();
	int height_timeRuler = this->sizes.height_timeRuler * screenSize.getHeight();
	int width_verticalScroller = this->sizes.width_verticalScroller * screenSize.getWidth();
	int height_horizontalScroller = this->sizes.height_horizontalScroller * screenSize.getHeight();

	//调整时间标尺大小
	this->timeRuler->setBounds(
		width_trackHead, 0,
		this->getWidth() - width_trackHead - width_verticalScroller, height_timeRuler);

	//调整垂直卷滚条大小
	this->vScroller->setBounds(
		this->getWidth() - width_verticalScroller, height_timeRuler,
		width_verticalScroller, this->getHeight() - height_timeRuler - height_horizontalScroller
	);

	//调整水平卷滚条大小
	this->hScroller->setBounds(
		width_trackHead, this->getHeight() - height_horizontalScroller,
		this->getWidth() - width_trackHead - width_verticalScroller, height_horizontalScroller
	);
}

void TrackEditor::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//填充背景
	g.fillAll(this->colors.background);

	//计算控件大小
	int width_trackHead = this->sizes.width_trackHead * screenSize.getWidth();
	int height_timeRuler = this->sizes.height_timeRuler * screenSize.getHeight();
	int height_borderTop = this->sizes.height_borderTop * screenSize.getHeight();
	int width_verticalScroller = this->sizes.width_verticalScroller * screenSize.getWidth();
	int height_horizontalScroller = this->sizes.height_horizontalScroller * screenSize.getHeight();
	int width_borderRight = this->sizes.width_borderRight * screenSize.getWidth();

	//绘制标尺头部
	juce::Rectangle<int> rectRulerHead(0, 0, width_trackHead, height_timeRuler);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectRulerHead);

	//绘制标尺头上边框
	juce::Rectangle<int> rectRulerHeadTopBorder(
		0, 0,
		width_trackHead, height_borderTop
	);
	g.setColour(this->colors.border);
	g.fillRect(rectRulerHeadTopBorder);

	//绘制标尺尾部
	juce::Rectangle<int> rectRulerTail(
		this->getWidth() - width_verticalScroller, 0,
		width_verticalScroller, height_timeRuler);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectRulerTail);

	//绘制标尺尾上边框
	juce::Rectangle<int> rectRulerTailTopBorder(
		this->getWidth() - width_verticalScroller, 0,
		width_verticalScroller, height_borderTop
	);
	g.setColour(this->colors.border);
	g.fillRect(rectRulerTailTopBorder);

	//绘制标尺尾右边框
	juce::Rectangle<int> rectRulerTailRightBorder(
		this->getWidth() - width_borderRight, 0,
		width_borderRight, height_timeRuler
	);
	g.setColour(this->colors.border);
	g.fillRect(rectRulerTailRightBorder);

	//绘制右下卷滚条交点
	juce::Rectangle<int> rectScrollerTail(
		this->getWidth() - width_verticalScroller, this->getHeight() - height_horizontalScroller,
		width_verticalScroller, height_horizontalScroller);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectScrollerTail);

	//绘制卷滚条交点右边框
	juce::Rectangle<int> rectScrollerTailRightBorder(
		this->getWidth() - width_borderRight, this->getHeight() - height_horizontalScroller,
		width_borderRight, height_horizontalScroller
	);
	g.setColour(this->colors.border);
	g.fillRect(rectScrollerTailRightBorder);

	//绘制下卷滚条头部
	juce::Rectangle<int> rectScrollerHead(
		0, this->getHeight() - height_horizontalScroller,
		width_trackHead, height_horizontalScroller);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectScrollerHead);

	//绘制卷滚条头部上边框
	juce::Rectangle<int> rectScrollerHeadTopBorder(
		0, this->getHeight() - height_horizontalScroller,
		width_trackHead, height_borderTop
	);
	g.setColour(this->colors.border);
	g.fillRect(rectScrollerHeadTopBorder);

	//绘制轨道头背景
	juce::Rectangle<int> rectTrackHead(
		0, height_timeRuler,
		width_trackHead, this->getHeight() - height_timeRuler - height_horizontalScroller);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectTrackHead);

	//绘制轨道头背景上边框
	juce::Rectangle<int> rectTrackHeadTopBorder(
		0, height_timeRuler,
		width_trackHead, height_borderTop);
	g.setColour(this->colors.border);
	g.fillRect(rectTrackHeadTopBorder);
}

void TrackEditor::setMethods(
	const std::function<void(int)>& setCurrentTrackFunc,
	const std::function<void(void)>& refreshTotalTimeFunc,
	const std::function<void(vocalshaper::ProjectTime)>& setCurrentPositionFunc,
	const std::function<void(vocalshaper::ProjectTime, vocalshaper::ProjectTime)>& setLoopRangeFunc,
	const std::function<void(vocalshaper::ProjectTime, vocalshaper::ProjectTime)>& setHorizontalViewPortFunc,
	const std::function<void(double, double)>& setVerticalViewPortFunc
)
{
	this->timeRuler->setMethods(
		setCurrentTrackFunc, refreshTotalTimeFunc, setCurrentPositionFunc,
		setLoopRangeFunc, setHorizontalViewPortFunc, setVerticalViewPortFunc);
	this->vScroller->setMethods(
		setCurrentTrackFunc, refreshTotalTimeFunc, setCurrentPositionFunc,
		setLoopRangeFunc, setHorizontalViewPortFunc, setVerticalViewPortFunc);
	this->hScroller->setMethods(
		setCurrentTrackFunc, refreshTotalTimeFunc, setCurrentPositionFunc,
		setLoopRangeFunc, setHorizontalViewPortFunc, setVerticalViewPortFunc);
	//TODO
	this->setTrackViewMethods(
		[this](vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime) {this->changeHViewPort(startTime, endTime); },
		[this](double bottomPer, double topPer) {this->changeVViewPort(bottomPer, topPer); }
	);
	this->EditorBase::setMethods(
		setCurrentTrackFunc, refreshTotalTimeFunc, setCurrentPositionFunc,
		setLoopRangeFunc, setHorizontalViewPortFunc, setVerticalViewPortFunc);
}

void TrackEditor::setTrackViewMethods(
	std::function<void(vocalshaper::ProjectTime, vocalshaper::ProjectTime)> setHViewPortFunc,
	std::function<void(double, double)> setVViewPortFunc
)
{
	this->timeRuler->setTrackViewMethods(setHViewPortFunc, setVViewPortFunc);
	this->vScroller->setTrackViewMethods(setHViewPortFunc, setVViewPortFunc);
	this->hScroller->setTrackViewMethods(setHViewPortFunc, setVViewPortFunc);
	//TODO
	this->EditorBase::setTrackViewMethods(setHViewPortFunc, setVViewPortFunc);
}

void TrackEditor::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);
	this->timeRuler->projectChanged(ptr);
	this->vScroller->projectChanged(ptr);
	this->hScroller->projectChanged(ptr);
	//TODO 刷新
}

void TrackEditor::setEditMode(bool editMode)
{
	this->editModeFlag = editMode;
	this->timeRuler->setEditMode(editMode);
	this->vScroller->setEditMode(editMode);
	this->hScroller->setEditMode(editMode);
}

void TrackEditor::setToolID(uint8_t toolID)
{
	this->toolID = toolID;
	this->timeRuler->setToolID(toolID);
	this->vScroller->setToolID(toolID);
	this->hScroller->setToolID(toolID);
}

void TrackEditor::trackChanged(int trackID)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->trackID = trackID;
	this->timeRuler->trackChanged(trackID);
	this->vScroller->trackChanged(trackID);
	this->hScroller->trackChanged(trackID);
	//TODO 刷新
}

void TrackEditor::setTotalLength(vocalshaper::ProjectTime totalLength)
{
	this->timeRuler->setTotalLength(totalLength);
	this->vScroller->setTotalLength(totalLength);
	this->hScroller->setTotalLength(totalLength);
	//TODO
}

void TrackEditor::setCurrentPosition(vocalshaper::ProjectTime currentTime)
{
	this->timeRuler->setCurrentPosition(currentTime);
	this->vScroller->setCurrentPosition(currentTime);
	this->hScroller->setCurrentPosition(currentTime);
	//TODO
}

void TrackEditor::setFollowState(bool follow)
{
	this->timeRuler->setFollowState(follow);
	this->vScroller->setFollowState(follow);
	this->hScroller->setFollowState(follow);
	//TODO
}

void TrackEditor::setLoopRange(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime)
{
	this->timeRuler->setLoopRange(startTime, endTime);
	this->vScroller->setLoopRange(startTime, endTime);
	this->hScroller->setLoopRange(startTime, endTime);
	//TODO
}

void TrackEditor::setHorizontalViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime)
{
	this->timeRuler->setHorizontalViewPort(startTime, endTime);
	this->vScroller->setHorizontalViewPort(startTime, endTime);
	this->hScroller->setHorizontalViewPort(startTime, endTime);
	//TODO
}

void TrackEditor::setVerticalViewPort(double bottomPitch, double topPitch)
{
	this->timeRuler->setVerticalViewPort(bottomPitch, topPitch);
	this->vScroller->setVerticalViewPort(bottomPitch, topPitch);
	this->hScroller->setVerticalViewPort(bottomPitch, topPitch);
	//TODO
}

void TrackEditor::setHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime)
{
	this->startTimeTemp = startTime;
	this->endTimeTemp = endTime;
	this->timeRuler->setHViewPort(startTime, endTime);
	this->vScroller->setHViewPort(startTime, endTime);
	this->hScroller->setHViewPort(startTime, endTime);
	//TODO
}

void TrackEditor::setVViewPort(double bottomTrack, double topTrack)
{
	this->bottomVTrackTemp = bottomTrack;
	this->topVTrackTemp = topTrack;
	this->timeRuler->setVViewPort(bottomTrack, topTrack);
	this->vScroller->setVViewPort(bottomTrack, topTrack);
	this->hScroller->setVViewPort(bottomTrack, topTrack);
	//TODO
}

void TrackEditor::setAdsorb(vocalshaper::AdsorbState state)
{
	this->timeRuler->setAdsorb(state);
	this->vScroller->setAdsorb(state);
	this->hScroller->setAdsorb(state);
	//TODO
}

void TrackEditor::setGrid(vocalshaper::GridState state)
{
	this->timeRuler->setGrid(state);
	this->vScroller->setGrid(state);
	this->hScroller->setGrid(state);
	//TODO
}

void TrackEditor::changeHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime)
{
	if (this->startTimeTemp == startTime && this->endTimeTemp == endTime) {
		return;
	}
	juce::ScopedReadLock locker1(this->projectLock);
	juce::ScopedReadLock locker2(this->project->getLock());
	auto project = this->project->getPtr();
	if (!project) {
		return;
	}
	if (vocalshaper::timeLSS(startTime, endTime, vocalshaper::ProjectDAO::getCurveQuantification(project))) {
		this->setHViewPort(startTime, endTime);
	}
}

void TrackEditor::changeVViewPort(double bottomTrack, double topTrack)
{
	if (this->bottomVTrackTemp == bottomTrack && this->topVTrackTemp == topTrack) {
		return;
	}
	if (bottomTrack > topTrack) {
		this->setVViewPort(bottomTrack, topTrack);
	}
}
