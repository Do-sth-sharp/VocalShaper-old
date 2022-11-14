#include "TrackView.h"
#include <libJModule.h>

TrackView::TrackView(std::function<void(double, double)> wheelChangeHMethod,
	std::function<void(double, double)> wheelChangeWithCtrlHMethod,
	std::function<void(double, double)> wheelChangeVMethod,
	std::function<void(double, double)> wheelChangeWithCtrlVMethod,
	std::function<void(const vocalshaper::Track*, bool)> showCurveMethod)
	: EditorBase(),
	wheelChangeHMethod(wheelChangeHMethod),
	wheelChangeWithCtrlHMethod(wheelChangeWithCtrlHMethod),
	wheelChangeVMethod(wheelChangeVMethod),
	wheelChangeWithCtrlVMethod(wheelChangeWithCtrlVMethod),
	showCurveMethod(showCurveMethod)
{
	this->setOpaque(false);

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
		"main", "color", "border", this->colors.border, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHead", this->sizes.width_trackHead, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-curveByTrack", this->scales.height_curveByTrack, result
		);

	//resource
}

void TrackView::setTrack(const vocalshaper::Track* track, bool isMaster)
{
	this->track = track;
	this->isMaster = isMaster;
	this->repaint();
}

const vocalshaper::Track* TrackView::getTrack()
{
	return this->track;
}

bool TrackView::getIsMaster()
{
	return this->isMaster;
}

void TrackView::setCurveShown(bool show)
{
	this->curveShown = show;
	this->repaint();
}

bool TrackView::isCurveShown()
{
	return this->curveShown;
}

int TrackView::getCurveSize()
{
	if (this->track) {
		return vocalshaper::TrackDAO::curveSize(this->track);
	}
	return 0;
}

void TrackView::resized()
{

}

void TrackView::paint(juce::Graphics& g)
{

}

void TrackView::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算判定区大小
	int width_head = this->sizes.width_trackHead * screenSize.getWidth();

	//获取鼠标指针属性
	float posX = event.position.getX();
	float posY = event.position.getY();

	//计算delta
	double delta = details.deltaY / 20.;

	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock projDataLocker(this->getProject()->getLock());

		//获取浏览范围
		double startTime = 0.;
		double endTime = 0.;
		std::tie(startTime, endTime) = this->getHViewPort();

		double topTrack = 0.;
		double bottomTrack = 0.;
		std::tie(bottomTrack, topTrack) = this->getVViewPort();

		if (posX >= width_head) {
			//计算时间
			double per = (posX - width_head) / (double)(this->getWidth() - width_head);

			//ctrl按下
			if (event.mods.isCtrlDown()) {
				this->wheelChangeWithCtrlHMethod(per, delta);
			}
			else {
				this->wheelChangeHMethod(per, delta);
			}
		}
		else {
			//计算轨道
			double per = (this->getBoundsInParent().getY() + posY)
				/ (double)this->getParentHeight();

			//ctrl按下
			if (event.mods.isCtrlDown()) {
				this->wheelChangeWithCtrlVMethod(per, delta);
			}
			else {
				this->wheelChangeVMethod(per, delta);
			}
		}
	}
}
