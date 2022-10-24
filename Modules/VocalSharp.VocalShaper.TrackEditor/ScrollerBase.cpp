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

	//position
	//scale

	//resource
}

bool ScrollerBase::getVertical()
{
	return this->isVertical;
}

void ScrollerBase::limitSize(double& sp, double& ep)
{
	if (ep < sp) { std::swap(ep, sp); }
	double delta = ep - sp;
	if (delta >= 1.) {
		sp = 0.;
		ep = 1.;
	}
	else {
		if (sp < 0.) { sp = 0.; ep = delta; }
		if (ep > 1.) { ep = 1.; sp = 1 - delta; }
	}
}

void ScrollerBase::paintPreView(juce::Graphics& g)
{
}

void ScrollerBase::noticeChange(double sp, double ep)
{
}

void ScrollerBase::refreshSizeOnTrackSizeChanged(
	int lastSize, int size, double& sp, double& ep)
{
}

void ScrollerBase::refreshSizeOnProjectLengthChanged(
	uint32_t lastLength, uint32_t length, double& sp, double& ep)
{
}

//
void ScrollerBase::resized()
{

}

void ScrollerBase::paint(juce::Graphics& g)
{

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
	double delta = details.deltaY / 50.;
	//TODO delta调优

	//计算编辑
	if (this->ptrTemp) {
		switch (this->scrollerState)
		{
		case ScrollerState::SPChange:
		{
			//更改值
			this->ptrTemp->ep = this->ptrTemp->ep + delta;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
			break;
		}
		case ScrollerState::EPChange:
		{
			//更改值
			this->ptrTemp->sp = this->ptrTemp->sp + delta;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		case ScrollerState::BlockChange:
		{
			//更改值
			this->ptrTemp->sp = this->ptrTemp->sp - delta / 2;
			this->ptrTemp->ep = this->ptrTemp->ep + delta / 2;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		case ScrollerState::Normal:
		{
			//更改值
			if (event.mods.isCtrlDown()) {
				this->ptrTemp->sp = this->ptrTemp->sp - delta / 2;
				this->ptrTemp->ep = this->ptrTemp->ep + delta / 2;
			}
			else {
				this->ptrTemp->sp = this->ptrTemp->sp + delta;
				this->ptrTemp->ep = this->ptrTemp->ep + delta;
			}

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

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

				//更改值
				this->ptrTemp->sp = pos;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

				//发送改变
				this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
			}
			else if (pos >= EPJAreaS && pos <= EPJAreaE) {
				//设状态
				this->scrollerState = ScrollerState::EPChange;

				//更改值
				this->ptrTemp->ep = pos;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

				//发送改变
				this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
			}
			else if (pos > SPJAreaE && pos < EPJAreaS) {
				//设状态
				this->scrollerState = ScrollerState::BlockChange;
				this->spTemp = this->ptrTemp->sp;
				this->epTemp = this->ptrTemp->ep;
				this->blockPerTemp = (pos - this->spTemp) / (this->epTemp - this->spTemp);

				//更改值
				double length = this->epTemp - this->spTemp;
				double delta = length * this->blockPerTemp;
				this->ptrTemp->sp = pos - delta;
				this->ptrTemp->ep = this->ptrTemp->sp + length;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

				//发送改变
				this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
			}
			else if (pos >= 0. && pos < SPJAreaS) {
				//设状态
				this->scrollerState = ScrollerState::BlockChange;
				this->spTemp = this->ptrTemp->sp;
				this->epTemp = this->ptrTemp->ep;
				this->blockPerTemp = 0.5;

				//更改值
				double length = this->epTemp - this->spTemp;
				double delta = length * this->blockPerTemp;
				this->ptrTemp->sp = pos - delta;
				this->ptrTemp->ep = this->ptrTemp->sp + length;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

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
				double length = this->epTemp - this->spTemp;
				double delta = length * this->blockPerTemp;
				this->ptrTemp->sp = pos - delta;
				this->ptrTemp->ep = this->ptrTemp->sp + length;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

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
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			break;
		}
		case ScrollerState::EPChange:
		{
			//更改值
			this->ptrTemp->ep = pos;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

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
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

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
		if (this->ptrTemp) {
			switch (this->scrollerState)
			{
			case ScrollerState::SPChange:
			{
				//更改值
				this->ptrTemp->sp = pos;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

				//发送改变
				this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

				break;
			}
			case ScrollerState::EPChange:
			{
				//更改值
				this->ptrTemp->ep = pos;

				//限制大小
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

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
				this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

				//发送改变
				this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

				break;
			}
			}
		}

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
	if (this->scrollerBlockHighlight || this->scrollerState != ScrollerState::Normal) {
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
void ScrollerBase::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker1(this->tempLock),locker2(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);

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
			juce::ScopedReadLock projLock(this->project->getLock());

			this->ptrTemp->followTemp = this->isFollow;

			this->ptrTemp->trackSizeTemp =
				vocalshaper::ProjectDAO::trackSize(this->project->getPtr());

			this->ptrTemp->projectCurveQuantTemp =
				vocalshaper::ProjectDAO::getCurveQuantification(this->project->getPtr());

			vocalshaper::ProjectTime totalLength =
				vocalshaper::CountTime::count(
					this->project->getPtr(), this->ptrTemp->projectCurveQuantTemp);
			uint32_t bar =
				this->project->getBeat()->getBarAtTime(
					vocalshaper::floor(totalLength, this->ptrTemp->projectCurveQuantTemp).first);
			bar = std::max(bar, 20Ui32) + 1;
			this->ptrTemp->projectLengthTemp =
				this->project->getBeat()->getTimeAtBar(bar);
		}

		//限制大小
		this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

		//发送改变
		this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
	}
	else {
		this->ptrTemp = nullptr;
		this->repaint();
	}
}

void ScrollerBase::trackChanged(int trackID)
{
	juce::ScopedWriteLock locker1(this->tempLock), locker2(this->projectLock);
	this->trackID = trackID;
	if (!this->project || !this->ptrTemp) {
		//刷新
		this->repaint();
		return;
	}

	//获取轨道数
	int trackSize = this->ptrTemp->trackSizeTemp;
	{
		juce::ScopedReadLock projLocker(this->project->getLock());
		trackSize = vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
	}

	if ((trackSize != this->ptrTemp->trackSizeTemp) && this->getVertical()) {
		//计算新缩放
		this->refreshSizeOnTrackSizeChanged(this->ptrTemp->trackSizeTemp, trackSize,
			this->ptrTemp->sp, this->ptrTemp->ep);

		//更新缓存
		this->ptrTemp->trackSizeTemp = trackSize;

		//限制大小
		this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

		//发送改变
		this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

		return;
	}
	
	//更新缓存
	this->ptrTemp->trackSizeTemp = trackSize;

	//刷新
	this->repaint();
}

void ScrollerBase::setHViewPort(vocalshaper::ProjectTime startTime, vocalshaper::ProjectTime endTime)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		if (!this->getVertical()) {
			//计算起止位置
			this->ptrTemp->sp =
				vocalshaper::timeToDouble(startTime, this->ptrTemp->projectCurveQuantTemp)
				/ this->ptrTemp->projectLengthTemp;
			this->ptrTemp->ep =
				vocalshaper::timeToDouble(endTime, this->ptrTemp->projectCurveQuantTemp)
				/ this->ptrTemp->projectLengthTemp;
		}
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setVViewPort(double bottomPer, double topPer)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		if (this->getVertical()) {
			//计算起止位置
			this->ptrTemp->sp = topPer;
			this->ptrTemp->ep = bottomPer;
		}
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setTotalLength(vocalshaper::ProjectTime totalLength)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		//取缓存
		auto trackSizeTemp = this->ptrTemp->trackSizeTemp;
		auto projectCurveQuantTemp = this->ptrTemp->projectCurveQuantTemp;
		auto projectLengthTemp = this->ptrTemp->projectLengthTemp;

		//计算工程相关信息
		{
			juce::ScopedReadLock projLock(this->project->getLock());

			trackSizeTemp =
				vocalshaper::ProjectDAO::trackSize(this->project->getPtr());

			projectCurveQuantTemp =
				vocalshaper::ProjectDAO::getCurveQuantification(this->project->getPtr());

			vocalshaper::ProjectTime totalLength =
				vocalshaper::CountTime::count(
					this->project->getPtr(), projectCurveQuantTemp);
			uint32_t bar =
				this->project->getBeat()->getBarAtTime(
					vocalshaper::floor(totalLength, projectCurveQuantTemp).first);
			bar = std::max(bar, 20Ui32) + 1;
			projectLengthTemp =
				this->project->getBeat()->getTimeAtBar(bar);
		}

		//如果是水平，则更新滑块大小
		if (!this->getVertical()) {
			//计算新缩放
			this->refreshSizeOnProjectLengthChanged(
				this->ptrTemp->projectLengthTemp, projectLengthTemp,
				this->ptrTemp->sp, this->ptrTemp->ep);

			//更新缓存
			this->ptrTemp->trackSizeTemp = trackSizeTemp;
			this->ptrTemp->projectCurveQuantTemp = projectCurveQuantTemp;
			this->ptrTemp->projectLengthTemp = projectLengthTemp;

			//限制大小
			this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

			//发送改变
			this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);

			return;
		}

		//更新缓存
		this->ptrTemp->trackSizeTemp = trackSizeTemp;
		this->ptrTemp->projectCurveQuantTemp = projectCurveQuantTemp;
		this->ptrTemp->projectLengthTemp = projectLengthTemp;
	}

	//刷新
	this->repaint();
}

void ScrollerBase::setCurrentPosition(vocalshaper::ProjectTime currentTime)
{
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		//更新缓存
		this->ptrTemp->currentPositionTemp =
			vocalshaper::timeToU64(currentTime, this->ptrTemp->projectCurveQuantTemp);

		if (!this->getVertical()) {
			//翻页
			if (this->ptrTemp->followTemp) {
				//计算当前百分比
				double per = this->ptrTemp->currentPositionTemp / (double)this->ptrTemp->projectLengthTemp;

				//需要翻页
				if (per < this->ptrTemp->sp || per >= this->ptrTemp->ep) {
					//计算翻页位置
					double diff = this->ptrTemp->ep - this->ptrTemp->sp;
					this->ptrTemp->sp = per;
					this->ptrTemp->ep = per + diff;

					//限制大小
					this->limitSize(this->ptrTemp->sp, this->ptrTemp->ep);

					//发送改变
					this->noticeChange(this->ptrTemp->sp, this->ptrTemp->ep);
				}
			}

			//刷新
			this->repaint();
		}
	}
}

void ScrollerBase::setFollowState(bool follow)
{
	this->isFollow = follow;
	juce::ScopedWriteLock locker1(this->tempLock);
	juce::ScopedReadLock locker2(this->projectLock);
	if (this->project && this->ptrTemp) {
		this->ptrTemp->followTemp = follow;
	}
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
