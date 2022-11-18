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

	//建立竖直卷滚条
	this->vScroller = std::make_unique<VScroller>();
	this->addChildEditorAndMakeVisible(this->vScroller.get());

	//建立水平卷滚条
	this->hScroller = std::make_unique<HScroller>();
	this->addChildEditorAndMakeVisible(this->hScroller.get());

	//建立时间标尺
	this->timeRuler = std::make_unique<TimeRuler>(
		[this](double per, double delta)
		{this->hScroller->sendWheelChange(per, delta); },
		[this](double per, double delta)
		{this->hScroller->sendWheelChangeWithCtrl(per, delta); });
	this->addChildEditorAndMakeVisible(this->timeRuler.get());

	//建立轨道列表
	this->tracks = std::make_unique<TrackList>(
		[this](double per, double delta)
		{this->hScroller->sendWheelChange(per, delta); },
		[this](double per, double delta)
		{this->hScroller->sendWheelChangeWithCtrl(per, delta); },
		[this](double per, double delta)
		{this->vScroller->sendWheelChange(per, delta); },
		[this](double per, double delta)
		{this->vScroller->sendWheelChangeWithCtrl(per, delta); },
		[this](const vocalshaper::Track* track, bool show)
		{this->vScroller->showCurve(track, show); },
		[this](const vocalshaper::Track* track)
		{return this->vScroller->curveIsShown(track); });
	this->addChildEditorAndMakeVisible(this->tracks.get());

	//设置回调
	this->vScroller->setCurveChangeCallback(
		[this] {this->tracks->resizeList(); });
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

	//调整轨道列表
	this->tracks->setBounds(
		0, height_timeRuler,
		this->getWidth() - width_verticalScroller,
		this->getHeight() - height_timeRuler - height_horizontalScroller
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
	float width_trackHead = this->sizes.width_trackHead * screenSize.getWidth();
	float height_timeRuler = this->sizes.height_timeRuler * screenSize.getHeight();
	float height_borderTop = this->sizes.height_borderTop * screenSize.getHeight();
	float width_verticalScroller = this->sizes.width_verticalScroller * screenSize.getWidth();
	float height_horizontalScroller = this->sizes.height_horizontalScroller * screenSize.getHeight();
	float width_borderRight = this->sizes.width_borderRight * screenSize.getWidth();

	//绘制标尺头部
	juce::Rectangle<float> rectRulerHead(0, 0, width_trackHead, height_timeRuler);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectRulerHead);

	//绘制标尺头上边框
	juce::Rectangle<float> rectRulerHeadTopBorder(
		0, 0,
		width_trackHead, height_borderTop
	);
	g.setColour(this->colors.border);
	g.fillRect(rectRulerHeadTopBorder);

	//绘制轨道头背景
	juce::Rectangle<float> rectTrackHead(
		0, height_timeRuler,
		width_trackHead, this->getHeight() - height_timeRuler - height_horizontalScroller);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectTrackHead);

	//绘制轨道头背景右边框
	juce::Rectangle<float> rectTrackHeadRightBorder(
		width_trackHead - width_borderRight, height_timeRuler,
		width_borderRight, this->getHeight() - height_timeRuler - height_horizontalScroller);
	g.setColour(this->colors.border);
	g.fillRect(rectTrackHeadRightBorder);

	//绘制轨道头背景上边框
	/*juce::Rectangle<int> rectTrackHeadTopBorder(
		0, height_timeRuler,
		width_trackHead, height_borderTop);
	g.setColour(this->colors.border);
	g.fillRect(rectTrackHeadTopBorder);*/

	//绘制标尺尾部
	juce::Rectangle<float> rectRulerTail(
		this->getWidth() - width_verticalScroller, 0,
		width_verticalScroller, height_timeRuler);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectRulerTail);

	//绘制标尺尾上边框
	juce::Rectangle<float> rectRulerTailTopBorder(
		this->getWidth() - width_verticalScroller, 0,
		width_verticalScroller, height_borderTop
	);
	g.setColour(this->colors.border);
	g.fillRect(rectRulerTailTopBorder);

	//绘制标尺尾右边框
	juce::Rectangle<float> rectRulerTailRightBorder(
		this->getWidth() - width_borderRight, 0,
		width_borderRight, height_timeRuler
	);
	g.setColour(this->colors.border);
	g.fillRect(rectRulerTailRightBorder);

	//绘制右下卷滚条交点
	juce::Rectangle<float> rectScrollerTail(
		this->getWidth() - width_verticalScroller, this->getHeight() - height_horizontalScroller,
		width_verticalScroller, height_horizontalScroller);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectScrollerTail);

	//绘制卷滚条交点右边框
	juce::Rectangle<float> rectScrollerTailRightBorder(
		this->getWidth() - width_borderRight, this->getHeight() - height_horizontalScroller,
		width_borderRight, height_horizontalScroller
	);
	g.setColour(this->colors.border);
	g.fillRect(rectScrollerTailRightBorder);

	//绘制下卷滚条头部
	juce::Rectangle<float> rectScrollerHead(
		0, this->getHeight() - height_horizontalScroller,
		width_trackHead, height_horizontalScroller);
	g.setColour(this->colors.background_rulerHead);
	g.fillRect(rectScrollerHead);

	//绘制卷滚条头部上边框
	juce::Rectangle<float> rectScrollerHeadTopBorder(
		0, this->getHeight() - height_horizontalScroller,
		width_trackHead, height_borderTop
	);
	g.setColour(this->colors.border);
	g.fillRect(rectScrollerHeadTopBorder);
}
