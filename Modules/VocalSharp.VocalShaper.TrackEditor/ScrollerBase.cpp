#include "ScrollerBase.h"
#include <libJModule.h>

ScrollerBase::ScrollerBase(bool isVertical)
	: EditorBase(), isVertical(isVertical)
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
		"main", "color", "background-scroller", this->colors.background_scroller, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "block-scroller", this->colors.block_scroller, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-scrollerBlockJudgeArea", this->sizes.width_scrollerBlockJudgeArea, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-scrollerBlockJudgeArea", this->sizes.height_scrollerBlockJudgeArea, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-scrollerMargin", this->sizes.width_scrollerMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-scrollerMargin", this->sizes.height_scrollerMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-scrollerBlockRadix", this->sizes.width_scrollerBlockRadix, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-scrollerBlockRadix", this->sizes.height_scrollerBlockRadix, result
		);

	//position
	//scale
	//resource

	//监听项目关闭
	jmadf::CallInterface<const vocalshaper::ProjectProxy::CloseCallbackFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddCloseCallback",
		[this](const vocalshaper::ProjectProxy* ptr) {this->listenProjectClose(ptr); });

	//监听曲线数量变化
	jmadf::CallInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules",
		[this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
		{this->listenCurveChange(action, type); });
}

bool ScrollerBase::getVertical() const
{
	return this->isVertical;
}

void ScrollerBase::showCurve(const vocalshaper::Track* track, bool show)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	juce::ScopedWriteLock locker(this->tempLock);
	if (this->getProject() && this->ptrTemp) {
		//复制缓存
		std::map<const vocalshaper::Track*, int> trackState = this->ptrTemp->trackState;
		{
			//更新缓存的曲线数量
			auto itState = trackState.find(track);
			if ((itState != trackState.end())) {
				itState->second = show ? vocalshaper::TrackDAO::curveSize(track) : 0;
			}
			else {
				trackState.insert(std::make_pair(track,
					show ? vocalshaper::TrackDAO::curveSize(track) : 0));
			}
		}

		if ((trackState != this->ptrTemp->trackState) && this->getVertical()) {
			//计算新缩放
			this->refreshSizeOnTrackSizeChanged(
				this->ptrTemp->trackSizeTemp, this->ptrTemp->trackSizeTemp,
				this->ptrTemp->trackState, trackState,
				this->ptrTemp->sp, this->ptrTemp->ep);

			//更新缓存
			this->ptrTemp->trackState = trackState;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			return;
		}

		//更新缓存
		this->ptrTemp->trackState = trackState;

		//刷新
		this->repaint();
	}
}

void ScrollerBase::sendWheelChange(double per, double delta)
{
	juce::ScopedWriteLock locker(this->tempLock);

	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	if (this->ptrTemp && this->scrollerState == ScrollerState::Normal) {
		if (!this->getVertical()) {
			//更改值
			this->ptrTemp->sp = this->ptrTemp->sp - delta;
			this->ptrTemp->ep = this->ptrTemp->ep - delta;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.5);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
		}
	}
}

void ScrollerBase::sendWheelChangeWithCtrl(double per, double delta)
{
	juce::ScopedWriteLock locker(this->tempLock);

	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	if (this->ptrTemp && this->scrollerState == ScrollerState::Normal) {
		if (!this->getVertical()) {
			if (per >= 0. && per <= 1.) {
				//更改值
				this->ptrTemp->sp = this->ptrTemp->sp - delta * per;
				this->ptrTemp->ep = this->ptrTemp->ep + delta * (1 - per);

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, per);
			}
			else {
				//更改值
				this->ptrTemp->sp = this->ptrTemp->sp - delta / 2;
				this->ptrTemp->ep = this->ptrTemp->ep + delta / 2;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.5);
			}

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
		}
	}
}

void ScrollerBase::limitSize(double& sp, double& ep, double nailPer)
{
	double delta = ep - sp;
	if (delta >= 1.) {
		sp = 0.;
		ep = 1.;
	}
	else {
		if (sp < 0.) { sp = 0.; if (nailPer > 0. && nailPer < 1.) { ep = delta; } }
		if (ep > 1.) { ep = 1.; if (nailPer > 0. && nailPer < 1.) { sp = 1 - delta; } }
	}
}

void ScrollerBase::paintPreView(juce::Graphics& g, int width, int height)
{
}

void ScrollerBase::noticeChange(double sp, double ep)
{
}

void ScrollerBase::refreshSizeOnTrackSizeChanged(
	int lastSize, int size,
	std::map<const vocalshaper::Track*, int> trackState, std::map<const vocalshaper::Track*, int> lastTrackState,
	double& sp, double& ep)
{
}

void ScrollerBase::refreshSizeOnProjectLengthChanged(
	double lastLength, double length, double& sp, double& ep)
{
}

void ScrollerBase::refreshSizeOnResized(int lastSize, int size, double& sp, double& ep)
{
}

void ScrollerBase::updateVViewPort(double bottomTrack, double topTrack, double& sp, double& ep)
{
}

//
void ScrollerBase::resized()
{
	//获得缓存
	int lastSize = this->sizeTemp;

	//判断是否需要更新并更新缓存
	bool shouldChange = false;
	if (this->getVertical()) {
		if (this->getHeight() != lastSize) {
			this->sizeTemp = this->getHeight();
			shouldChange = true;
		}
	}
	else {
		if (this->getWidth() != lastSize) {
			this->sizeTemp = this->getWidth();
			shouldChange = true;
		}
	}

	//不需要更新缓存，短路
	if (!shouldChange) {
		return;
	}

	//如果缓存有效，试图控制分辨率
	{
		juce::ScopedReadLock locker(this->tempLock);
		if (this->ptrTemp) {
			//根据缩放以控制分辨率为目的重算滑块区域
			this->refreshSizeOnResized(lastSize, this->sizeTemp, this->ptrTemp->sp, this->ptrTemp->ep);

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
		}
	}
}

void ScrollerBase::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//绘制背景
	g.setColour(this->colors.background_scroller);
	g.fillAll();

	//绘制预览
	this->paintPreView(g, this->getWidth(), this->getHeight());

	//根据状态选择颜色
	juce::Colour colorBlock = this->colors.block_scroller;
	if (this->scrollerState != ScrollerState::Normal) {
		colorBlock = colorBlock.withMultipliedAlpha(1.5f);
	}
	else if (this->scrollerBlockHighlight || this->scrollerBlockBorderHighlight) {
		colorBlock = colorBlock.withMultipliedAlpha(1.5f);
	}

	//计算控件大小
	int radix = 0, margin = 0, startPos = 0, endPos = 0;
	{
		juce::ScopedReadLock locker(this->tempLock);
		if (this->getVertical()) {
			radix = this->sizes.width_scrollerBlockRadix * screenSize.getWidth();
			margin = this->sizes.width_scrollerMargin * screenSize.getWidth();
			startPos = 0;
			endPos = this->getHeight();
			if (this->ptrTemp) {
				startPos = this->getHeight() * this->ptrTemp->sp;
				endPos = this->getHeight() * this->ptrTemp->ep;
			}
		}
		else {
			radix = this->sizes.height_scrollerBlockRadix * screenSize.getHeight();
			margin = this->sizes.height_scrollerMargin * screenSize.getHeight();
			startPos = 0;
			endPos = this->getWidth();
			if (this->ptrTemp) {
				startPos = this->getWidth() * this->ptrTemp->sp;
				endPos = this->getWidth() * this->ptrTemp->ep;
			}
		}
	}
	
	//绘制滑块
	g.setColour(colorBlock);
	if (this->getVertical()) {
		g.fillRoundedRectangle(
			margin, startPos,
			this->getWidth() - margin * 2, endPos - startPos,
			radix);
	}
	else {
		g.fillRoundedRectangle(
			startPos, margin,
			endPos - startPos, this->getHeight() - margin * 2,
			radix
		);
	}
}

//
void ScrollerBase::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details)
{
	juce::ScopedWriteLock locker(this->tempLock);

	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//考虑横竖计算鼠标位置和判定区域
	double pos = 0;
	double judge = 0;
	if (this->getVertical()) {
		pos = event.position.getY() / (double)this->getHeight();
		judge = (this->sizes.height_scrollerBlockJudgeArea * screenSize.getHeight()) / this->getHeight();
	}
	else {
		pos = event.position.getX() / (double)this->getWidth();
		judge = (this->sizes.width_scrollerBlockJudgeArea * screenSize.getWidth()) / this->getWidth();
	}
	
	//计算delta
	double delta = details.deltaY / 20.;

	//计算编辑
	if (this->ptrTemp) {
		switch (this->scrollerState)
		{
		case ScrollerState::SPChange:
		{
			//更改值
			this->ptrTemp->ep = this->ptrTemp->ep - delta;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
			break;
		}
		case ScrollerState::EPChange:
		{
			//更改值
			this->ptrTemp->sp = this->ptrTemp->sp - delta;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 1.);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		case ScrollerState::BlockChange:
		{
			double per = this->blockPerTemp;
			if (per >= 0. && per <= 1.) {
				//更改值
				this->ptrTemp->sp = this->ptrTemp->sp - delta * per;
				this->ptrTemp->ep = this->ptrTemp->ep + delta * (1 - per);

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, per);
			}
			else {
				//更改值
				this->ptrTemp->sp = this->ptrTemp->sp - delta / 2;
				this->ptrTemp->ep = this->ptrTemp->ep + delta / 2;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.5);
			}

			//更新缓存
			this->spTemp = this->ptrTemp->sp;
			this->epTemp = this->ptrTemp->ep;

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		case ScrollerState::Normal:
		{
			if (event.mods.isCtrlDown()) {
				double per = (pos - this->ptrTemp->sp) / (this->ptrTemp->ep - this->ptrTemp->sp);
				if (per >= 0. && per <= 1.) {
					//更改值
					this->ptrTemp->sp = this->ptrTemp->sp - delta * per;
					this->ptrTemp->ep = this->ptrTemp->ep + delta * (1 - per);

					//限制大小
					this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, per);
				}
				else {
					//更改值
					this->ptrTemp->sp = this->ptrTemp->sp - delta / 2;
					this->ptrTemp->ep = this->ptrTemp->ep + delta / 2;

					//限制大小
					this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.5);
				}
			}
			else {
				//更改值
				this->ptrTemp->sp = this->ptrTemp->sp - delta;
				this->ptrTemp->ep = this->ptrTemp->ep - delta;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.5);
			}

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		}
	}

	//根据更改状态计算区域状态
	this->scrollerBlockHighlight = false;
	this->scrollerBlockBorderHighlight = false;
	if (this->scrollerState == ScrollerState::Normal) {
		//如果非更改状态根据鼠标位置判断滑块高亮
		if (this->ptrTemp) {
			//计算判定区域
			bool haveBlockArea = (this->ptrTemp->ep - this->ptrTemp->sp) > judge;	//滑块判定区未被占用
			double SPJAreaS = this->ptrTemp->sp - judge / 2;//前判定区起始
			double SPJAreaE = haveBlockArea
				? (this->ptrTemp->sp + judge / 2)
				: (this->ptrTemp->sp + (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//前判定区结束
			double EPJAreaS = haveBlockArea
				? (this->ptrTemp->ep - judge / 2)
				: (this->ptrTemp->ep - (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//后判定区起始
			double EPJAreaE = this->ptrTemp->ep + judge / 2;//后判定区结束

			//根据判定区域设状态
			if ((pos >= SPJAreaS && pos <= SPJAreaE) || (pos >= EPJAreaS && pos <= EPJAreaE)) {
				this->scrollerBlockBorderHighlight = true;
			}
			else if (pos > SPJAreaE && pos < EPJAreaS) {
				this->scrollerBlockHighlight = true;
			}
		}
	}
	else {
		//如果更改状态直接判断滑块高亮
		switch (this->scrollerState)
		{
		case ScrollerState::SPChange:
		{
			this->scrollerBlockBorderHighlight = true;
			break;
		}
		case ScrollerState::EPChange:
		{
			this->scrollerBlockBorderHighlight = true;
			break;
		}
		case ScrollerState::BlockChange:
		{
			this->scrollerBlockHighlight = true;
			break;
		}
		}
	}

	//设置鼠标状态
	if (this->scrollerBlockBorderHighlight) {
		if (this->getVertical()) {
			this->setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
		}
		else {
			this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
		}
	}
	else if (this->scrollerBlockHighlight) {
		if (this->scrollerState == ScrollerState::Normal) {
			this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
		}
		else {
			this->setMouseCursor(juce::MouseCursor::DraggingHandCursor);
		}
	}
	if (!(this->scrollerBlockBorderHighlight || this->scrollerBlockHighlight)) {
		this->setMouseCursor(juce::MouseCursor::NormalCursor);
	}

	//刷新
	this->repaint();
}

void ScrollerBase::mouseDown(const juce::MouseEvent& event)
{
	juce::ScopedWriteLock locker(this->tempLock);

	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//考虑横竖计算鼠标位置和判定区域
	double pos = 0;
	double judge = 0;
	if (this->getVertical()) {
		pos = event.position.getY() / (double)this->getHeight();
		judge = (this->sizes.height_scrollerBlockJudgeArea * screenSize.getHeight()) / this->getHeight();
	}
	else {
		pos = event.position.getX() / (double)this->getWidth();
		judge = (this->sizes.width_scrollerBlockJudgeArea * screenSize.getWidth()) / this->getWidth();
	}

	//左键变化
	if (event.mods.isLeftButtonDown()) {
		if (this->ptrTemp) {
			//计算判定区域
			bool haveBlockArea = (this->ptrTemp->ep - this->ptrTemp->sp) > judge;	//滑块判定区未被占用
			double SPJAreaS = this->ptrTemp->sp - judge / 2;//前判定区起始
			double SPJAreaE = haveBlockArea
				? (this->ptrTemp->sp + judge / 2)
				: (this->ptrTemp->sp + (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//前判定区结束
			double EPJAreaS = haveBlockArea
				? (this->ptrTemp->ep - judge / 2)
				: (this->ptrTemp->ep - (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//后判定区起始
			double EPJAreaE = this->ptrTemp->ep + judge / 2;//后判定区结束

			if (pos >= SPJAreaS && pos <= SPJAreaE) {
				//设状态
				this->scrollerState = ScrollerState::SPChange;
			}
			else if (pos >= EPJAreaS && pos <= EPJAreaE) {
				//设状态
				this->scrollerState = ScrollerState::EPChange;
			}
			else if (pos > SPJAreaE && pos < EPJAreaS) {
				//设状态
				this->scrollerState = ScrollerState::BlockChange;
				this->spTemp = this->ptrTemp->sp;
				this->epTemp = this->ptrTemp->ep;
				this->blockPerTemp = (pos - this->spTemp) / (this->epTemp - this->spTemp);
			}
			else if (pos >= 0. && pos < SPJAreaS) {
				//设状态
				this->scrollerState = ScrollerState::BlockChange;
				this->spTemp = this->ptrTemp->sp;
				this->epTemp = this->ptrTemp->ep;
				this->blockPerTemp = 0.5;

				//更改值
				double delta = this->ptrTemp->ep - this->ptrTemp->sp;
				this->ptrTemp->sp = pos - delta / 2;
				this->ptrTemp->ep = pos + delta / 2;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.5);

				//更新缓存
				this->spTemp = this->ptrTemp->sp;
				this->epTemp = this->ptrTemp->ep;

				//发送改变
				this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
			}
			else if (pos > EPJAreaE && pos <= 1.) {
				//设状态
				this->scrollerState = ScrollerState::BlockChange;
				this->spTemp = this->ptrTemp->sp;
				this->epTemp = this->ptrTemp->ep;
				this->blockPerTemp = 0.5;

				//更改值
				double delta = this->ptrTemp->ep - this->ptrTemp->sp;
				this->ptrTemp->sp = pos - delta / 2;
				this->ptrTemp->ep = pos + delta / 2;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.5);

				//更新缓存
				this->spTemp = this->ptrTemp->sp;
				this->epTemp = this->ptrTemp->ep;

				//发送改变
				this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
			}
		}
	}

	//根据更改状态计算区域状态
	this->scrollerBlockHighlight = false;
	this->scrollerBlockBorderHighlight = false;
	if (this->scrollerState == ScrollerState::Normal) {
		//如果非更改状态根据鼠标位置判断滑块高亮
		if (this->ptrTemp) {
			//计算判定区域
			bool haveBlockArea = (this->ptrTemp->ep - this->ptrTemp->sp) > judge;	//滑块判定区未被占用
			double SPJAreaS = this->ptrTemp->sp - judge / 2;//前判定区起始
			double SPJAreaE = haveBlockArea
				? (this->ptrTemp->sp + judge / 2)
				: (this->ptrTemp->sp + (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//前判定区结束
			double EPJAreaS = haveBlockArea
				? (this->ptrTemp->ep - judge / 2)
				: (this->ptrTemp->ep - (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//后判定区起始
			double EPJAreaE = this->ptrTemp->ep + judge / 2;//后判定区结束

			//根据判定区域设状态
			if ((pos >= SPJAreaS && pos <= SPJAreaE) || (pos >= EPJAreaS && pos <= EPJAreaE)) {
				this->scrollerBlockBorderHighlight = true;
			}
			else if (pos > SPJAreaE && pos < EPJAreaS) {
				this->scrollerBlockHighlight = true;
			}
		}
	}
	else {
		//如果更改状态直接判断滑块高亮
		switch (this->scrollerState)
		{
		case ScrollerState::SPChange:
		{
			this->scrollerBlockBorderHighlight = true;
			break;
		}
		case ScrollerState::EPChange:
		{
			this->scrollerBlockBorderHighlight = true;
			break;
		}
		case ScrollerState::BlockChange:
		{
			this->scrollerBlockHighlight = true;
			break;
		}
		}
	}

	//设置鼠标状态
	if (this->scrollerBlockBorderHighlight) {
		if (this->getVertical()) {
			this->setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
		}
		else {
			this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
		}
	}
	else if (this->scrollerBlockHighlight) {
		if (this->scrollerState == ScrollerState::Normal) {
			this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
		}
		else {
			this->setMouseCursor(juce::MouseCursor::DraggingHandCursor);
		}
	}
	if (!(this->scrollerBlockBorderHighlight || this->scrollerBlockHighlight)) {
		this->setMouseCursor(juce::MouseCursor::NormalCursor);
	}
	
	//刷新
	this->repaint();
}

void ScrollerBase::mouseMove(const juce::MouseEvent& event)
{
	juce::ScopedWriteLock locker(this->tempLock);

	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//考虑横竖计算鼠标位置和判定区域
	double pos = 0;
	double judge = 0;
	if (this->getVertical()) {
		pos = event.position.getY() / (double)this->getHeight();
		judge = (this->sizes.height_scrollerBlockJudgeArea * screenSize.getHeight()) / this->getHeight();
	}
	else {
		pos = event.position.getX() / (double)this->getWidth();
		judge = (this->sizes.width_scrollerBlockJudgeArea * screenSize.getWidth()) / this->getWidth();
	}

	//正在编辑
	if (this->ptrTemp) {
		switch (this->scrollerState)
		{
		case ScrollerState::SPChange:
		{
			//更改值
			this->ptrTemp->sp = pos;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 1.);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		case ScrollerState::EPChange:
		{
			//更改值
			this->ptrTemp->ep = pos;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		case ScrollerState::BlockChange:
		{
			//更改值
			double length = this->epTemp - this->spTemp;
			double delta = length * this->blockPerTemp;
			this->ptrTemp->sp = pos - delta;
			this->ptrTemp->ep = this->ptrTemp->sp + length;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, this->blockPerTemp);

			//更新缓存
			this->spTemp = this->ptrTemp->sp;
			this->epTemp = this->ptrTemp->ep;

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		}
	}

	//根据更改状态计算区域状态
	this->scrollerBlockHighlight = false;
	this->scrollerBlockBorderHighlight = false;
	if (this->scrollerState == ScrollerState::Normal) {
		//如果非更改状态根据鼠标位置判断滑块高亮
		if (this->ptrTemp) {
			//计算判定区域
			bool haveBlockArea = (this->ptrTemp->ep - this->ptrTemp->sp) > judge;	//滑块判定区未被占用
			double SPJAreaS = this->ptrTemp->sp - judge / 2;//前判定区起始
			double SPJAreaE = haveBlockArea
				? (this->ptrTemp->sp + judge / 2)
				: (this->ptrTemp->sp + (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//前判定区结束
			double EPJAreaS = haveBlockArea
				? (this->ptrTemp->ep - judge / 2)
				: (this->ptrTemp->ep - (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//后判定区起始
			double EPJAreaE = this->ptrTemp->ep + judge / 2;//后判定区结束

			//根据判定区域设状态
			if ((pos >= SPJAreaS && pos <= SPJAreaE) || (pos >= EPJAreaS && pos <= EPJAreaE)) {
				this->scrollerBlockBorderHighlight = true;
			}
			else if (pos > SPJAreaE && pos < EPJAreaS) {
				this->scrollerBlockHighlight = true;
			}
		}
	}
	else {
		//如果更改状态直接判断滑块高亮
		switch (this->scrollerState)
		{
		case ScrollerState::SPChange:
		{
			this->scrollerBlockBorderHighlight = true;
			break;
		}
		case ScrollerState::EPChange:
		{
			this->scrollerBlockBorderHighlight = true;
			break;
		}
		case ScrollerState::BlockChange:
		{
			this->scrollerBlockHighlight = true;
			break;
		}
		}
	}
	
	//设置鼠标状态
	if (this->scrollerBlockBorderHighlight) {
		if (this->getVertical()) {
			this->setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
		}
		else {
			this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
		}
	}
	else if (this->scrollerBlockHighlight) {
		if (this->scrollerState == ScrollerState::Normal) {
			this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
		}
		else {
			this->setMouseCursor(juce::MouseCursor::DraggingHandCursor);
		}
	}
	if (!(this->scrollerBlockBorderHighlight || this->scrollerBlockHighlight)) {
		this->setMouseCursor(juce::MouseCursor::NormalCursor);
	}

	//刷新
	this->repaint();
}

void ScrollerBase::mouseDrag(const juce::MouseEvent& event)
{
	this->mouseMove(event);
}

void ScrollerBase::mouseUp(const juce::MouseEvent& event)
{
	juce::ScopedWriteLock locker(this->tempLock);

	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//考虑横竖计算鼠标位置和判定区域
	double pos = 0;
	double judge = 0;
	if (this->getVertical()) {
		pos = event.position.getY() / (double)this->getHeight();
		judge = (this->sizes.height_scrollerBlockJudgeArea * screenSize.getHeight()) / this->getHeight();
	}
	else {
		pos = event.position.getX() / (double)this->getWidth();
		judge = (this->sizes.width_scrollerBlockJudgeArea * screenSize.getWidth()) / this->getWidth();
	}

	//左键变化
	if (event.mods.isLeftButtonDown()) {
		//还原状态
		this->scrollerState = ScrollerState::Normal;
		this->spTemp = 0.;
		this->epTemp = 1.;
		this->blockPerTemp = 0.5;
	}

	//根据鼠标位置判断滑块高亮
	this->scrollerBlockHighlight = false;
	this->scrollerBlockBorderHighlight = false;
	if (this->ptrTemp) {
		//计算判定区域
		bool haveBlockArea = (this->ptrTemp->ep - this->ptrTemp->sp) > judge;	//滑块判定区未被占用
		double SPJAreaS = this->ptrTemp->sp - judge / 2;//前判定区起始
		double SPJAreaE = haveBlockArea
			? (this->ptrTemp->sp + judge / 2)
			: (this->ptrTemp->sp + (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//前判定区结束
		double EPJAreaS = haveBlockArea
			? (this->ptrTemp->ep - judge / 2)
			: (this->ptrTemp->ep - (this->ptrTemp->ep - this->ptrTemp->sp) / 2);//后判定区起始
		double EPJAreaE = this->ptrTemp->ep + judge / 2;//后判定区结束

		//根据判定区域设状态
		if ((pos >= SPJAreaS && pos <= SPJAreaE) || (pos >= EPJAreaS && pos <= EPJAreaE)) {
			this->scrollerBlockBorderHighlight = true;
		}
		else if (pos > SPJAreaE && pos < EPJAreaS) {
			this->scrollerBlockHighlight = true;
		}
	}
	
	//设置鼠标状态
	if (this->scrollerBlockBorderHighlight) {
		if (this->getVertical()) {
			this->setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
		}
		else {
			this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
		}
	}
	else if (this->scrollerBlockHighlight) {
		this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	}
	if (!(this->scrollerBlockBorderHighlight || this->scrollerBlockHighlight)) {
		this->setMouseCursor(juce::MouseCursor::NormalCursor);
	}
	
	//刷新
	this->repaint();
}

void ScrollerBase::mouseExit(const juce::MouseEvent& event)
{
	//重置全部状态并刷新
	if (this->scrollerBlockHighlight || this->scrollerBlockBorderHighlight || this->scrollerState != ScrollerState::Normal) {
		this->scrollerBlockHighlight = false;
		this->scrollerBlockBorderHighlight = false;
		this->setMouseCursor(juce::MouseCursor::NormalCursor);
		this->scrollerState = ScrollerState::Normal;
		this->spTemp = 0.;
		this->epTemp = 1.;
		this->blockPerTemp = 0.5;
		this->repaint();
	}
}

//
void ScrollerBase::setProjectCallback(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker1(this->tempLock),locker2(this->getProjLock());

	if (ptr) {
		//获取或建立缓存
		auto it = this->tempList.find(ptr);
		if (it != this->tempList.end()) {
			this->ptrTemp = &(it->second);
		}
		else {
			this->ptrTemp = &(this->tempList[ptr]);
		}

		//缓存工程相关信息
		{
			juce::ScopedReadLock projLock(this->getProject()->getLock());

			this->ptrTemp->followTemp = this->getFollowState();

			this->ptrTemp->trackSizeTemp =
				vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr());

			double totalLength =
				vocalshaper::CountTime::count(this->getProject()->getPtr());
			double bar =
				this->getProject()->getBeat()->getBarAtTime(std::floor(totalLength));
			bar = std::max(std::floor(bar) + 4, 20.);
			this->ptrTemp->projectLengthTemp =
				this->getProject()->getBeat()->getTimeAtBar(bar);
		}

		//限制大小
		this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

		//发送改变
		this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
	}
	else {
		this->ptrTemp = nullptr;
		this->repaint();
	}
}

void ScrollerBase::setTrackIDCallback(int trackID)
{
	juce::ScopedWriteLock locker1(this->tempLock), locker2(this->getProjLock());
	if (!this->getProject() || !this->ptrTemp) {
		//刷新
		this->repaint();
		return;
	}

	//获取轨道数
	int trackSize = this->ptrTemp->trackSizeTemp;
	std::map<const vocalshaper::Track*, int> trackState = this->ptrTemp->trackState;
	{
		juce::ScopedReadLock projLocker(this->getProject()->getLock());

		//获取轨道总数
		trackSize = vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr());

		//移除无效缓存，更新有效缓存的曲线数量
		std::set<const vocalshaper::Track*> trackSet;
		for (int i = 0; i < trackSize; i++) {
			trackSet.insert(vocalshaper::ProjectDAO::getTrack(this->getProject()->getPtr(), i));
		}
		trackSet.insert(vocalshaper::ProjectDAO::getMasterTrack(this->getProject()->getPtr()));
		for (auto it = trackState.begin(); it != trackState.end();) {
			if (trackSet.count(it->first) > 0) {
				if (it->second > 0) {
					it->second = vocalshaper::TrackDAO::curveSize(it->first);
				}
			}
			else {
				it = trackState.erase(it);
				continue;
			}
			it++;
		}
	}

	if (((trackSize != this->ptrTemp->trackSizeTemp) || trackState != this->ptrTemp->trackState) &&
		this->getVertical()) {
		//计算新缩放
		this->refreshSizeOnTrackSizeChanged(this->ptrTemp->trackSizeTemp, trackSize,
			this->ptrTemp->trackState, trackState,
			this->ptrTemp->sp, this->ptrTemp->ep);

		//更新缓存
		this->ptrTemp->trackSizeTemp = trackSize;
		this->ptrTemp->trackState = trackState;

		//限制大小
		this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

		//发送改变
		this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

		return;
	}
	
	//更新缓存
	this->ptrTemp->trackSizeTemp = trackSize;
	this->ptrTemp->trackState = trackState;

	//刷新
	this->repaint();
}

void ScrollerBase::setHViewPortCallback(double startTime, double endTime)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->getProjLock());
	if (this->getProject() && this->ptrTemp) {
		if (!this->getVertical()) {
			//计算起止位置
			this->ptrTemp->sp = startTime / this->ptrTemp->projectLengthTemp;
			this->ptrTemp->ep = endTime / this->ptrTemp->projectLengthTemp;
		}
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setVViewPortCallback(double bottomTrack, double topTrack)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->getProjLock());
	if (this->getProject() && this->ptrTemp) {
		if (this->getVertical()) {
			//计算起止位置
			this->updateVViewPort(bottomTrack, topTrack,
				this->ptrTemp->sp, this->ptrTemp->ep);
		}
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setTotalLengthCallback(double totalLength)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->getProjLock());
	if (this->getProject() && this->ptrTemp) {
		//取缓存
		auto trackSizeTemp = this->ptrTemp->trackSizeTemp;
		auto projectLengthTemp = this->ptrTemp->projectLengthTemp;

		//计算工程相关信息
		{
			juce::ScopedReadLock projLock(this->getProject()->getLock());

			trackSizeTemp =
				vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr());

			double bar =
				this->getProject()->getBeat()->getBarAtTime(std::floor(totalLength));
			bar = std::max(std::floor(bar) + 4, 20.);
			projectLengthTemp =
				this->getProject()->getBeat()->getTimeAtBar(bar);
		}

		//如果是水平，则更新滑块大小
		if (!this->getVertical()) {
			//计算新缩放
			this->refreshSizeOnProjectLengthChanged(
				this->ptrTemp->projectLengthTemp, projectLengthTemp,
				this->ptrTemp->sp, this->ptrTemp->ep);

			//更新缓存
			this->ptrTemp->trackSizeTemp = trackSizeTemp;
			this->ptrTemp->projectLengthTemp = projectLengthTemp;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			return;
		}

		//更新缓存
		this->ptrTemp->trackSizeTemp = trackSizeTemp;
		this->ptrTemp->projectLengthTemp = projectLengthTemp;
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setCurrentPositionCallback(double currentTime)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->getProjLock());
	if (this->getProject() && this->ptrTemp) {
		//更新缓存
		this->ptrTemp->currentPositionTemp = currentTime;

		if (!this->getVertical()) {
			//翻页
			if (this->ptrTemp->followTemp) {
				//计算当前百分比
				double per = this->ptrTemp->currentPositionTemp / (double)this->ptrTemp->projectLengthTemp;

				//需要翻页
				if (per < this->ptrTemp->sp || per > this->ptrTemp->ep) {
					//计算翻页位置
					double diff = this->ptrTemp->ep - this->ptrTemp->sp;
					this->ptrTemp->sp = per;
					this->ptrTemp->ep = per + diff;
					if (this->ptrTemp->ep > 1.) {
						this->ptrTemp->ep = 1.;
						this->ptrTemp->sp = this->ptrTemp->ep - diff;
					}

					//限制大小
					this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

					//发送改变
					this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
				}
			}

			//刷新
			this->repaint();
		}
	}
}

void ScrollerBase::setFollowStateCallback(bool follow)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->getProjLock());
	if (this->getProject() && this->ptrTemp) {
		this->ptrTemp->followTemp = follow;
	}
}

void ScrollerBase::setLoopRangeCallback(double startTime, double endTime)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->getProjLock());
	if (this->getProject() && this->ptrTemp) {
		this->ptrTemp->loopST = startTime;
		this->ptrTemp->loopET = endTime;
	}
	this->repaint();
}

void ScrollerBase::listenProjectClose(const vocalshaper::ProjectProxy* ptr)
{
	//清除对应缓存
	juce::ScopedWriteLock locker(this->tempLock);
	auto it = this->tempList.find(ptr);
	if (it != this->tempList.end()) {
		if (this->ptrTemp == &(it->second)) {
			this->ptrTemp = nullptr;
		}
		this->tempList.erase(it);
	}
	this->repaint();
}

void ScrollerBase::listenCurveChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject() != action.getProxy()) {
		return;
	}
	if (action.getBaseType() == vocalshaper::actions::ActionBase::Type::Track) {
		if (
			action.getActionType() == vocalshaper::actions::TrackAction::Actions::AddCurve ||
			action.getActionType() == vocalshaper::actions::TrackAction::Actions::RemoveCurve) {
			juce::ScopedWriteLock locker(this->tempLock);
			if (this->getProject() && this->ptrTemp) {
				//获取消息队列
				auto messageManager = juce::MessageManager::getInstance();
				if (!messageManager) {
					return;
				}

				//复制缓存
				std::map<const vocalshaper::Track*, int> trackState = this->ptrTemp->trackState;
				{
					//获取目标轨道
					const vocalshaper::Track* track = nullptr;
					auto target =
						reinterpret_cast<const vocalshaper::actions::TrackAction::TargetType*>(
							action.getTarget());
					if (target->track > -1) {
						track = vocalshaper::ProjectDAO::getMasterTrack(
							this->getProject()->getPtr());
					}
					else {
						track = vocalshaper::ProjectDAO::getTrack(
							this->getProject()->getPtr(), target->track);
					}

					//更新缓存的曲线数量
					auto itState = trackState.find(track);
					if ((itState != trackState.end()) && (itState->second > 0)) {
						itState->second = vocalshaper::TrackDAO::curveSize(track);
					}
				}

				if ((trackState != this->ptrTemp->trackState) && this->getVertical()) {
					//计算新缩放
					this->refreshSizeOnTrackSizeChanged(
						this->ptrTemp->trackSizeTemp, this->ptrTemp->trackSizeTemp,
						this->ptrTemp->trackState, trackState,
						this->ptrTemp->sp, this->ptrTemp->ep);

					//更新缓存
					this->ptrTemp->trackState = trackState;

					//限制大小
					this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep, 0.);

					//发送改变
					double sp = this->ptrTemp->sp, ep = this->ptrTemp->ep;
					messageManager->callAsync([sp, ep, this] {this->noticeChange(sp, ep); });
					
					return;
				}

				//更新缓存
				this->ptrTemp->trackState = trackState;

				//刷新
				messageManager->callAsync([this] {this->repaint(); });
			}
		}
	}
}
