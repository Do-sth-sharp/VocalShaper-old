#include "TrackList.h"
#include <libJModule.h>

TrackList::TrackList(
	std::function<void(double, double)> wheelChangeHMethod,
	std::function<void(double, double)> wheelChangeWithCtrlHMethod,
	std::function<void(double, double)> wheelChangeVMethod,
	std::function<void(double, double)> wheelChangeWithCtrlVMethod,
	std::function<void(const vocalshaper::Track*, bool)> showCurveMethod,
	std::function<bool(const vocalshaper::Track*)> curveIsShownMethod)
	: EditorBase(),
	wheelChangeHMethod(wheelChangeHMethod),
	wheelChangeWithCtrlHMethod(wheelChangeWithCtrlHMethod),
	wheelChangeVMethod(wheelChangeVMethod),
	wheelChangeWithCtrlVMethod(wheelChangeWithCtrlVMethod),
	showCurveMethod(showCurveMethod),
	curveIsShownMethod(curveIsShownMethod)
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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "cursor", this->colors.cursor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "fill-trackList-timeUnselect", this->colors.fill_trackList_timeUnselect, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHead", this->sizes.width_trackHead, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-cursor", this->sizes.width_cursor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-borderTop", this->sizes.height_borderTop, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-curveByTrack", this->scales.height_curveByTrack, result
		);

	//resource

	//监听颜色变化
	jmadf::CallInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules",
		[this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
		{this->listenColorChange(action, type); });

	//监听MS状态变化
	jmadf::CallInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules",
		[this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
		{this->listenSMChange(action, type); });
}

void TrackList::resized()
{
	this->resizeList();
}

void TrackList::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float height_borderTop = this->sizes.height_borderTop * screenSize.getHeight();
	int width_head = this->sizes.width_trackHead * screenSize.getWidth();

	//获取轨道状态
	double bottomTrack = 0., topTrack = 0.;
	std::tie(bottomTrack, topTrack) = this->getVViewPort();
	double ppt = this->getHeight() / (bottomTrack - topTrack);
	double startTime = 0., endTime = 0.;
	std::tie(startTime, endTime) = this->getHViewPort();
	double ppq = (this->getWidth() - width_head) / (endTime - startTime);

	//绘背景网格
	
	//绘制选区遮盖
	{
		double loopStartTime = -1., loopEndTime = -1.;
		std::tie(loopStartTime, loopEndTime) = this->getLoopRange();
		if (loopStartTime < loopEndTime) {
			//绘前半
			if (loopStartTime > startTime) {
				float pos = width_head + (loopStartTime - startTime) * ppq;
				juce::Rectangle<float> rectFill(
					width_head, 0,
					pos - width_head, this->getHeight()
				);
				g.setColour(this->colors.fill_trackList_timeUnselect);
				g.fillRect(rectFill);
			}

			//绘后半
			if (loopEndTime < endTime) {
				float pos = width_head + (loopEndTime - startTime) * ppq;
				juce::Rectangle<float> rectFill(
					pos, 0,
					this->getWidth() - pos, this->getHeight()
				);
				g.setColour(this->colors.fill_trackList_timeUnselect);
				g.fillRect(rectFill);
			}
		}
	}

	//绘制全区域上边框
	{
		juce::Rectangle<float> rectBorder(
			0, 0, this->getWidth(), height_borderTop);
		g.setColour(this->colors.border);
		g.fillRect(rectBorder);
	}

	//绘制轨道上边框
	{
		juce::ScopedReadLock listLocker(this->listLock);

		//绘制轨道上边框
		int posYView = -(topTrack * ppt);
		for (int i = 0; i < this->trackList.size(); i++) {
			//计算位置
			auto trackView = this->trackList.getUnchecked(i);
			int heightView = ppt;
			if (trackView->isCurveShown()) {
				heightView += (ppt * this->scales.height_curveByTrack)
					* trackView->getCurveSize();
			}

			//绘制边框
			juce::Rectangle<float> rectBorder(
				0, posYView, this->getWidth(), height_borderTop);
			g.setColour(this->colors.border);
			g.fillRect(rectBorder);

			//缓存增长
			posYView += heightView;
		}

		//绘制末边框
		{
			juce::Rectangle<float> rectBorder(
				0, posYView, this->getWidth(), height_borderTop);
			g.setColour(this->colors.border);
			g.fillRect(rectBorder);
		}
	}
}

void TrackList::paintOverChildren(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//绘制播放指针
	{
		//计算轨道头大小
		int width_head = this->sizes.width_trackHead * screenSize.getWidth();

		//获取轨道状态
		double startTime = 0., endTime = 0.;
		std::tie(startTime, endTime) = this->getHViewPort();
		double ppq = (this->getWidth() - width_head) / (endTime - startTime);
		double currentTime = this->getCurrentPosition();

		if (currentTime >= startTime && currentTime <= endTime) {
			//计算指针位置
			float width_cursor = this->sizes.width_cursor * screenSize.getWidth();
			float pos = width_head + (currentTime - startTime) * ppq;

			//绘制指针
			juce::Rectangle<float> rectCursor(
				pos - width_cursor / 2, 0,
				width_cursor, this->getHeight()
			);
			g.setColour(this->colors.cursor);
			g.fillRect(rectCursor);
		}
	}
	
}

void TrackList::refreshList()
{
	juce::ScopedWriteLock listLocker(this->listLock);

	//清理轨道视图
	{
		for (auto i : this->trackList) {
			this->removeChildEditor(i);
		}
		this->trackList.clear();
	}

	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock proxyLocker(this->getProject()->getLock());

		//获取轨道总数
		int trackSize = vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr());

		//主轨
		{
			auto trackView = new TrackView(
				this->wheelChangeHMethod,
				this->wheelChangeWithCtrlHMethod,
				this->wheelChangeVMethod,
				this->wheelChangeWithCtrlVMethod,
				this->showCurveMethod
			);
			auto track = 
				vocalshaper::ProjectDAO::getMasterTrack(this->getProject()->getPtr());
			trackView->setTrack(track, -1, true);
			trackView->setCurveShown(this->curveIsShownMethod(track));
			this->addChildEditorAndMakeVisible(trackView);
			this->trackList.add(trackView);
		}

		//其余轨
		for (int i = 0; i < trackSize; i++) {
			auto trackView = new TrackView(
				this->wheelChangeHMethod,
				this->wheelChangeWithCtrlHMethod,
				this->wheelChangeVMethod,
				this->wheelChangeWithCtrlVMethod,
				this->showCurveMethod
			);
			auto track =
				vocalshaper::ProjectDAO::getTrack(this->getProject()->getPtr(), i);
			trackView->setTrack(track, i);
			trackView->setCurveShown(this->curveIsShownMethod(track));
			this->addChildEditorAndMakeVisible(trackView);
			this->trackList.add(trackView);
		}

		//排布
		this->resizeList();
	}
	else {
		//更新绘图
		this->repaint();
	}
}

void TrackList::resizeList()
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock proxyLocker(this->getProject()->getLock());
		juce::ScopedReadLock listLocker(this->listLock);

		//获取轨道状态
		double bottomTrack = 0., topTrack = 0.;
		std::tie(bottomTrack, topTrack) = this->getVViewPort();
		double ppt = this->getHeight() / (bottomTrack - topTrack);

		//排布预览器
		int posYView = -(topTrack * ppt);
		for (int i = 0; i < this->trackList.size(); i++) {
			auto trackView = this->trackList.getUnchecked(i);
			int heightView = ppt;
			if (trackView->isCurveShown()) {
				heightView += (ppt * this->scales.height_curveByTrack)
					* trackView->getCurveSize();
			}

			trackView->setBounds(0, posYView, this->getWidth(), heightView);
			posYView += heightView;
		}
	}

	//更新绘图
	this->repaint();
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
	this->refreshList();
}

void TrackList::setTrackIDCallback(int trackID)
{
	this->refreshList();
}

void TrackList::setCurrentPositionCallback(double currentTime)
{
	this->repaint();
}

void TrackList::setLoopRangeCallback(double startTime, double endTime)
{
	this->repaint();
}

void TrackList::setHViewPortCallback(double startTime, double endTime)
{
	this->repaint();
}

void TrackList::setVViewPortCallback(double bottomTrack, double topTrack)
{
	this->resizeList();
}

void TrackList::setGridCallback(vocalshaper::GridState state)
{
	//TODO 更新背景网格
}

void TrackList::listenColorChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock listLocker(this->listLock);
	for (auto i : this->trackList) {
		i->listenColorChange(action, type);
	}
}

void TrackList::listenSMChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock listLocker(this->listLock);
	for (auto i : this->trackList) {
		i->listenSMChange(action, type);
	}
}
