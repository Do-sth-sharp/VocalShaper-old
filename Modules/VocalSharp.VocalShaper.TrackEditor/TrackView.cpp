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

	//获取翻译器
	jmadf::CallInterface<std::function<const juce::String(const juce::String&)>&>(
		"WuChang.JMADF.Translates", "GetFunc",
		this->tr
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
		"main", "color", "text-trackHeadName", this->colors.text_trackHeadName, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-MSButton", this->colors.text_MSButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-MSButton", this->colors.background_MSButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-MSButton-highlight", this->colors.text_MSButton_highlight, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHead", this->sizes.width_trackHead, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHeadLeftMargin", this->sizes.width_trackHeadLeftMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHeadRightMargin", this->sizes.width_trackHeadRightMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-trackHeadTopMargin", this->sizes.height_trackHeadTopMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-trackHeadBottomMargin", this->sizes.height_trackHeadBottomMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-trackHeadTitleAndButtonLine", this->sizes.height_trackHeadTitleAndButtonLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-trackHeadNameText", this->sizes.height_trackHeadNameText, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHeadColorLightLeftMargin", this->sizes.width_trackHeadColorLightLeftMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHeadLightNameSplit", this->sizes.width_trackHeadLightNameSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHeadNameButtonSplit", this->sizes.width_trackHeadNameButtonSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHeadButtonSplit", this->sizes.width_trackHeadButtonSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-trackHeadLineSplit", this->sizes.height_trackHeadLineSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-trackHeadControlLine", this->sizes.height_trackHeadControlLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHeadLinkCurveSplit", this->sizes.width_trackHeadLinkCurveSplit, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-curveByTrack", this->scales.height_curveByTrack, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-trackHeadColorLight", this->scales.height_trackHeadColorLight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-trackHeadMSButton", this->scales.height_trackHeadMSButton, result
		);

	//resource

	//建立MS按钮样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetTrackViewSMButtonLAF",
		this->lafs.SMButton
		);
	this->lafs.SMButton->setColour(
		juce::TextButton::ColourIds::textColourOffId, this->colors.text_MSButton
	);
	this->lafs.SMButton->setColour(
		juce::TextButton::ColourIds::textColourOnId, this->colors.text_MSButton_highlight
	);
	this->lafs.SMButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_MSButton
	);
	this->lafs.SMButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//建立MS按钮
	this->mButton = std::make_unique<juce::TextButton>("M");
	this->mButton->setToggleable(true);
	this->mButton->setWantsKeyboardFocus(false);
	this->mButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->mButton->setLookAndFeel(this->lafs.SMButton);
	this->mButton->onClick = [this] {
		this->sendMute(!this->mButton->getToggleState());
	};
	this->addAndMakeVisible(this->mButton.get());

	this->sButton = std::make_unique<juce::TextButton>("S");
	this->sButton->setToggleable(true);
	this->sButton->setWantsKeyboardFocus(false);
	this->sButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->sButton->setLookAndFeel(this->lafs.SMButton);
	this->sButton->onClick = [this] {
		this->sendSolo(!this->sButton->getToggleState());
	};
	this->addAndMakeVisible(this->sButton.get());

	//建立链接按钮
	this->linkButton = std::make_unique<juce::TextButton>("");
	this->addChildComponent(this->linkButton.get());

	//建立曲线显示按钮
	this->curveButton = std::make_unique<juce::DrawableButton>(
		"Show Curve", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->addChildComponent(this->curveButton.get());
}

void TrackView::setTrack(const vocalshaper::Track* track, int index, bool isMaster)
{
	this->track = track;
	this->index = index;
	this->isMaster = isMaster;

	this->setColor(vocalshaper::TrackDAO::getColour(track));

	this->mButton->setToggleState(vocalshaper::TrackDAO::getMute(track),
		juce::NotificationType::dontSendNotification);
	this->sButton->setToggleState(vocalshaper::TrackDAO::getSolo(track),
		juce::NotificationType::dontSendNotification);

	this->repaint();
}

const vocalshaper::Track* TrackView::getTrack()
{
	return this->track;
}

int TrackView::getIndex()
{
	return this->index;
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
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float width_head = this->sizes.width_trackHead * screenSize.getWidth();

	//轨道头
	{
		//计算控件大小
		float height_marginTop = this->sizes.height_trackHeadTopMargin * screenSize.getHeight();
		float height_marginBottom = this->sizes.height_trackHeadBottomMargin * screenSize.getHeight();
		float width_marginLeft = this->sizes.width_trackHeadLeftMargin * screenSize.getWidth();
		float width_marginRight = this->sizes.width_trackHeadRightMargin * screenSize.getWidth();

		float height_titleLine = this->sizes.height_trackHeadTitleAndButtonLine * screenSize.getHeight();
		float height_controlLine = this->sizes.height_trackHeadControlLine * screenSize.getHeight();

		float height_lineSplit = this->sizes.height_trackHeadLineSplit * screenSize.getHeight();

		float width_lightLeftMargin = this->sizes.width_trackHeadColorLightLeftMargin * screenSize.getWidth();
		float width_lightNameSplit = this->sizes.width_trackHeadLightNameSplit * screenSize.getWidth();
		float width_nameButtonSplit = this->sizes.width_trackHeadNameButtonSplit * screenSize.getWidth();
		float width_buttonSplit = this->sizes.width_trackHeadButtonSplit * screenSize.getWidth();
		float width_linkCurveSplit = this->sizes.width_trackHeadLinkCurveSplit * screenSize.getWidth();

		float height_titleText = this->sizes.height_trackHeadNameText * screenSize.getHeight();

		float height_colorLight = this->scales.height_trackHeadColorLight * height_titleLine;
		float height_msButton = this->scales.height_trackHeadMSButton * height_titleLine;
		float width_colorLight = height_colorLight;
		float width_msButton = height_msButton;

		float width_curveButton = height_controlLine;

		//调整MS按钮位置
		this->mButton->setBounds(
			width_head - width_marginRight - width_buttonSplit - width_msButton * 2,
			height_marginTop + height_titleLine / 2 - height_msButton / 2,
			width_msButton, height_msButton
		);
		this->sButton->setBounds(
			width_head - width_marginRight - width_msButton,
			height_marginTop + height_titleLine / 2 - height_msButton / 2,
			width_msButton, height_msButton
		);

		//调整链接按钮位置
		this->linkButton->setBounds(
			width_marginLeft, height_marginTop + height_titleLine + height_lineSplit,
			width_head - width_marginLeft - width_marginRight - width_curveButton - width_linkCurveSplit,
			height_controlLine
		);

		//调整曲线显示按钮位置
		this->curveButton->setBounds(
			width_head - width_marginRight - width_curveButton,
			height_marginTop + height_titleLine + height_lineSplit,
			width_curveButton, height_controlLine
		);

		//根据高度计算下行显示
		bool controlLineShow = (this->getHeight() >=
			height_marginTop + height_titleLine + height_lineSplit + height_controlLine + height_marginBottom);
		this->linkButton->setVisible(controlLineShow);
		this->curveButton->setVisible(controlLineShow);
	}
}

void TrackView::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float width_head = this->sizes.width_trackHead * screenSize.getWidth();

	//轨道头
	{
		//计算控件大小
		float height_marginTop = this->sizes.height_trackHeadTopMargin * screenSize.getHeight();
		float height_marginBottom = this->sizes.height_trackHeadBottomMargin * screenSize.getHeight();
		float width_marginLeft = this->sizes.width_trackHeadLeftMargin * screenSize.getWidth();
		float width_marginRight = this->sizes.width_trackHeadRightMargin * screenSize.getWidth();

		float height_titleLine = this->sizes.height_trackHeadTitleAndButtonLine * screenSize.getHeight();
		float height_controlLine = this->sizes.height_trackHeadControlLine * screenSize.getHeight();

		float height_lineSplit = this->sizes.height_trackHeadLineSplit * screenSize.getHeight();

		float width_lightLeftMargin = this->sizes.width_trackHeadColorLightLeftMargin * screenSize.getWidth();
		float width_lightNameSplit = this->sizes.width_trackHeadLightNameSplit * screenSize.getWidth();
		float width_nameButtonSplit = this->sizes.width_trackHeadNameButtonSplit * screenSize.getWidth();
		float width_buttonSplit = this->sizes.width_trackHeadButtonSplit * screenSize.getWidth();
		float width_linkCurveSplit = this->sizes.width_trackHeadLinkCurveSplit * screenSize.getWidth();

		float height_titleText = this->sizes.height_trackHeadNameText * screenSize.getHeight();

		float height_colorLight = this->scales.height_trackHeadColorLight * height_titleLine;
		float height_msButton = this->scales.height_trackHeadMSButton * height_titleLine;
		float width_colorLight = height_colorLight;
		float width_msButton = height_msButton;

		//生成字体
		auto titleFont = g.getCurrentFont();
		titleFont.setHeight(height_titleText);

		juce::ScopedReadLock projLock(this->getProjLock());
		if (this->getProject() && this->getTrack()) {
			juce::ScopedReadLock proxyLock(this->getProject()->getLock());

			{
				//获取轨道颜色
				auto trackColor = vocalshaper::TrackDAO::getColour(this->getTrack());

				//绘颜色指示
				juce::Rectangle<float> rectLightArea(
					width_marginLeft + width_lightLeftMargin + width_colorLight / 2,
					height_marginTop + height_titleLine / 2 - height_colorLight / 2,
					width_colorLight, height_colorLight
				);
				g.setColour(trackColor);
				g.fillEllipse(rectLightArea);
			}

			{
				//获取轨道名称
				auto trackName = vocalshaper::TrackDAO::getName(this->getTrack());
				if (trackName.isEmpty()) {
					if (this->getIsMaster()) {
						trackName = this->tr("tip_UntitledMasterTrack");
					}
					else {
						trackName = this->tr("tip_UntitledTrackLead") + " " + juce::String(this->getIndex() + 1);
					}
				}

				//绘制轨道文字
				float title_posX =
					width_marginLeft + width_lightLeftMargin + width_colorLight + width_lightNameSplit;
				juce::Rectangle<int> rectTitle(
					title_posX, height_marginTop,
					width_head - title_posX - width_marginRight - width_msButton * 2 - width_buttonSplit - width_nameButtonSplit,
					height_titleLine
				);
				g.setFont(titleFont);
				g.setColour(this->colors.text_trackHeadName);
				g.drawFittedText(trackName, rectTitle,
					juce::Justification::centredLeft, 1, 1.f);
			}
		}
	}
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

void TrackView::listenColorChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	if (action.getBaseType() == vocalshaper::actions::ActionBase::Type::Track &&
		action.getActionType() == vocalshaper::actions::TrackAction::ActionType::Colour) {
		if (action.getProxy() == this->getProject()) {
			juce::ScopedReadLock projLocker(this->getProjLock());
			if (this->getProject() && this->getTrack()) {
				juce::ScopedReadLock proxyLocker(this->getProject()->getLock());

				auto target =
					reinterpret_cast<vocalshaper::actions::TrackAction::TargetType*>(action.getTarget());
				auto track = vocalshaper::ProjectDAO::getTrack(action.getProxy()->getPtr(), target->track);
				if (this->getTrack() == track) {
					auto color = vocalshaper::TrackDAO::getColour(track);
					
					auto messageManager = juce::MessageManager::getInstance();
					if (!messageManager) {
						return;
					}
					
					messageManager->callAsync([this, color] {this->setColor(color); });
				}
			}
		}
	}
}

void TrackView::listenSMChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	if (action.getBaseType() == vocalshaper::actions::ActionBase::Type::Track &&
		(action.getActionType() == vocalshaper::actions::TrackAction::ActionType::Solo ||
			action.getActionType() == vocalshaper::actions::TrackAction::ActionType::Mute)) {
		if (action.getProxy() == this->getProject()) {
			juce::ScopedReadLock projLocker(this->getProjLock());
			if (this->getProject() && this->getTrack()) {
				juce::ScopedReadLock proxyLocker(this->getProject()->getLock());

				auto target =
					reinterpret_cast<vocalshaper::actions::TrackAction::TargetType*>(action.getTarget());
				auto track = vocalshaper::ProjectDAO::getTrack(action.getProxy()->getPtr(), target->track);
				if (target->track == -1) {
					track = vocalshaper::ProjectDAO::getMasterTrack(action.getProxy()->getPtr());
				}
				if (this->getTrack() == track) {
					auto solo = vocalshaper::TrackDAO::getSolo(track);
					auto mute = vocalshaper::TrackDAO::getMute(track);

					auto messageManager = juce::MessageManager::getInstance();
					if (!messageManager) {
						return;
					}

					messageManager->callAsync(
						[this, solo] {this->sButton->setToggleState(solo, juce::NotificationType::dontSendNotification); });
					messageManager->callAsync(
						[this, mute] {this->mButton->setToggleState(mute, juce::NotificationType::dontSendNotification); });
				}
			}
		}
	}
}

void TrackView::setColor(juce::Colour color)
{
	this->lafs.SMButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId,
		color
	);
	this->mButton->repaint();
	this->sButton->repaint();
}

void TrackView::sendSolo(bool solo)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		using ActionType = vocalshaper::actions::track::SoloAction;
		ActionType::TargetType target{};
		target.track = this->getIndex();
		auto action = std::make_unique<ActionType>(
			target, solo, this->getProject()
			);

		//发送事件
		this->getProject()->getProcesser()->processEvent(std::move(action));
	}
}

void TrackView::sendMute(bool mute)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		using ActionType = vocalshaper::actions::track::MuteAction;
		ActionType::TargetType target{};
		target.track = this->getIndex();
		auto action = std::make_unique<ActionType>(
			target, mute, this->getProject()
			);

		//发送事件
		this->getProject()->getProcesser()->processEvent(std::move(action));
	}
}