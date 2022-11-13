#include "TrackList.h"
#include <libJModule.h>

TrackList::TrackList(
	std::function<void(double, double)> wheelChangeHMethod,
	std::function<void(double, double)> wheelChangeWithCtrlHMethod,
	std::function<void(double, double)> wheelChangeVMethod,
	std::function<void(double, double)> wheelChangeWithCtrlVMethod)
	: EditorBase(),
	wheelChangeHMethod(wheelChangeHMethod),
	wheelChangeWithCtrlHMethod(wheelChangeWithCtrlHMethod),
	wheelChangeVMethod(wheelChangeVMethod),
	wheelChangeWithCtrlVMethod(wheelChangeWithCtrlVMethod)
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

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHead", this->sizes.width_trackHead, result
		);

	//position
	//scale

	//resource
}

void TrackList::resized()
{

}

void TrackList::paint(juce::Graphics& g)
{

}

void TrackList::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details)
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
			double per = posY / (double)this->getHeight();

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

void TrackList::setProjectCallback(const vocalshaper::ProjectProxy* ptr)
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

void TrackList::setTrackIDCallback(int trackID)
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
