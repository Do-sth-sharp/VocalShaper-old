#include "TrackView.h"
#include <libJModule.h>
#include "TrackViewSMButtonLAF.h"
#include "TrackViewLinkButtonLAF.h"

TrackView::TrackView(std::function<void(double, double)> wheelChangeHMethod,
	std::function<void(double, double)> wheelChangeWithCtrlHMethod,
	std::function<void(double, double)> wheelChangeVMethod,
	std::function<void(double, double)> wheelChangeWithCtrlVMethod,
	std::function<void(const vocalshaper::Track*, bool)> showCurveMethod,
	std::function<int(const vocalshaper::Track*)> getCurveSizeMethod)
	: EditorBase(),
	wheelChangeHMethod(wheelChangeHMethod),
	wheelChangeWithCtrlHMethod(wheelChangeWithCtrlHMethod),
	wheelChangeVMethod(wheelChangeVMethod),
	wheelChangeWithCtrlVMethod(wheelChangeWithCtrlVMethod),
	showCurveMethod(showCurveMethod),
	getCurveSizeMethod(getCurveSizeMethod)
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
	this->initUIConfigAndIcon();

	//建立文本编辑器样式
	this->lafs.textEditor = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.textEditor->setColour(
		juce::TextEditor::ColourIds::textColourId, this->colors.text_trackNameEditor
	);
	this->lafs.textEditor->setColour(
		juce::TextEditor::ColourIds::backgroundColourId, this->colors.background_trackNameEditor
	);
	this->lafs.textEditor->setColour(
		juce::TextEditor::ColourIds::highlightedTextColourId, this->colors.text_trackNameEditor_highlight
	);
	this->lafs.textEditor->setColour(
		juce::TextEditor::ColourIds::highlightColourId, this->colors.background_trackNameEditor_highlight
	);
	this->lafs.textEditor->setColour(
		juce::TextEditor::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);
	this->lafs.textEditor->setColour(
		juce::TextEditor::ColourIds::focusedOutlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);
	this->lafs.textEditor->setColour(
		juce::TextEditor::ColourIds::shadowColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);
	this->lafs.textEditor->setColour(
		juce::CaretComponent::ColourIds::caretColourId, this->colors.caret_trackNameEditor
	);

	//建立文本编辑框
	this->nameEditor = std::make_unique<juce::TextEditor>();
	this->nameEditor->setLookAndFeel(this->lafs.textEditor.get());
	this->nameEditor->setHasFocusOutline(false);
	this->nameEditor->setMultiLine(false);
	this->nameEditor->setJustification(juce::Justification::centredLeft);
	this->nameEditor->setClicksOutsideDismissVirtualKeyboard(true);
	this->nameEditor->setPopupMenuEnabled(false);
	this->nameEditor->onReturnKey = [this] {
		this->nameEditor->focusLost(juce::Component::FocusChangeType::focusChangedDirectly);
	};
	this->nameEditor->onFocusLost = [this] {
		this->sendName(this->nameEditor->getText());
	};
	this->addAndMakeVisible(this->nameEditor.get());

	//建立MS按钮样式
	this->lafs.SMButton = std::unique_ptr<juce::LookAndFeel>(new TrackViewSMButtonLAF);
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
	this->mButton = std::make_unique<juce::TextButton>("M", this->tr("tip_Mute"));
	this->mButton->setToggleable(true);
	this->mButton->setWantsKeyboardFocus(false);
	this->mButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->mButton->setLookAndFeel(this->lafs.SMButton.get());
	this->mButton->onClick = [this] {
		this->sendMute(!this->mButton->getToggleState());
	};
	this->addAndMakeVisible(this->mButton.get());

	this->sButton = std::make_unique<juce::TextButton>("S", this->tr("tip_Solo"));
	this->sButton->setToggleable(true);
	this->sButton->setWantsKeyboardFocus(false);
	this->sButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->sButton->setLookAndFeel(this->lafs.SMButton.get());
	this->sButton->onClick = [this] {
		this->sendSolo(!this->sButton->getToggleState());
	};
	this->addAndMakeVisible(this->sButton.get());

	//建立链接按钮样式
	this->lafs.linkButton = std::unique_ptr<juce::LookAndFeel>(new TrackViewLinkButtonLAF);
	this->lafs.linkButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_trackViewLinkButton
	);
	this->lafs.linkButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_trackViewLinkButton
	);
	this->lafs.linkButton->setColour(
		juce::TextButton::ColourIds::textColourOnId, this->colors.text_trackViewLinkButton
	);
	this->lafs.linkButton->setColour(
		juce::TextButton::ColourIds::textColourOffId, this->colors.text_trackViewLinkButton
	);
	this->lafs.linkButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//建立链接按钮
	this->linkButton = std::make_unique<juce::TextButton>(
		this->tr("bt_EmptyTrack"), this->tr("tip_Link"));
	this->linkButton->setWantsKeyboardFocus(false);
	this->linkButton->setFocusContainerType(juce::Component::FocusContainerType::none);
	this->linkButton->setLookAndFeel(this->lafs.linkButton.get());
	this->linkButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->linkButton->onClick = [this] {this->showLinkMenu(); };
	this->addChildComponent(this->linkButton.get());

	//建立曲线显示按钮样式
	this->lafs.showCurveButton = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.showCurveButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_trackViewShowCurveButton
	);
	this->lafs.showCurveButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_trackViewShowCurveButton_highlight
	);
	this->lafs.showCurveButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//建立曲线显示按钮
	this->curveButton = std::make_unique<juce::DrawableButton>(
		"Show Curve", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->curveButton->setImages(this->iconCurve.get(), nullptr, nullptr, nullptr,
		this->iconCurveHighlight.get(), nullptr, nullptr, nullptr);
	this->curveButton->setToggleable(true);
	this->curveButton->setLookAndFeel(this->lafs.showCurveButton.get());
	this->curveButton->setWantsKeyboardFocus(false);
	this->curveButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->curveButton->onClick = [this] {
		this->showCurve(!this->isCurveShown());
	};
	this->addChildComponent(this->curveButton.get());

	//建立颜色选择器
	this->colorEditor = std::make_unique<ColorEditorCallOutBox>(this);
}

void TrackView::setTrack(const vocalshaper::Track* track, int index, bool isMaster)
{
	this->track = track;
	this->index = index;
	this->isMaster = isMaster;

	this->setColor(vocalshaper::TrackDAO::getColour(track));
	this->setTrackName(vocalshaper::TrackDAO::getName(track));

	{
		juce::String emptyText;
		if (isMaster) {
			emptyText = this->tr("tip_UntitledMasterTrack");
		}
		else {
			emptyText = this->tr("tip_UntitledTrackLead") + " " + juce::String(index + 1);
		}
		this->nameEditor->setTextToShowWhenEmpty(emptyText, this->colors.text_trackNameEditor_empty);
	}

	this->mButton->setToggleState(vocalshaper::TrackDAO::getMute(track),
		juce::NotificationType::dontSendNotification);
	this->sButton->setToggleState(vocalshaper::TrackDAO::getSolo(track),
		juce::NotificationType::dontSendNotification);

	this->mButton->setEnabled(!isMaster);
	this->sButton->setEnabled(!isMaster);

	{
		juce::ScopedReadLock projLocker(this->getProjLock());
		if (this->getProject()) {
			juce::ScopedReadLock proxyLocker(this->getProject()->getLock());
			this->setTrackType(track);
		}
	}

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
	this->curveButton->setToggleState(show, juce::NotificationType::dontSendNotification);
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

bool TrackView::isSelected()
{
	return (!this->getIsMaster()) && (this->getIndex() == this->getTrackID());
}

void TrackView::resized()
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float width_head = this->sizes.width_trackHead * screenSize.getWidth();

	//计算轨道主体大小与曲线编辑器大小
	float height_track = this->getHeight();
	if (this->isCurveShown()) {
		int curveSize = this->getCurveSize();
		height_track = this->getHeight() / (1 + curveSize * this->scales.height_curveByTrack);
	}

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

		//生成字体
		auto titleFont = this->nameEditor->getFont();
		titleFont.setHeight(height_titleText);

		//设置标题字体
		this->nameEditor->setFont(titleFont);

		//调整标题位置
		float title_posX =
			width_marginLeft + width_lightLeftMargin + width_colorLight + width_lightNameSplit;
		this->nameEditor->setBounds(
			title_posX, height_marginTop,
			width_head - title_posX - width_marginRight - width_msButton * 2 - width_buttonSplit - width_nameButtonSplit,
			height_titleLine
		);

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
		bool controlLineShow = (height_track >=
			height_marginTop + height_titleLine + height_lineSplit + height_controlLine + height_marginBottom);
		this->linkButton->setVisible(controlLineShow);
		this->curveButton->setVisible(controlLineShow);
	}

	//颜色选择器
	{
		//计算控件大小
		int width_colorEditor = this->sizes.width_colorEditor * screenSize.getWidth();
		int height_colorEditor = this->sizes.height_colorEditor * screenSize.getHeight();
		float width_colorEditorArrow = this->sizes.width_colorEditorCalloutArrow * screenSize.getWidth();
		int width_colorEditorBorder = this->sizes.width_colorEditorCalloutBorder * screenSize.getWidth();
		float width_colorEditorCorner = this->sizes.width_colorEditorCalloutCorner * screenSize.getWidth();

		//调整大小
		this->colorEditor->resize(width_colorEditor, height_colorEditor);
		this->colorEditor->setArrowSize(width_colorEditorArrow);
		this->colorEditor->setBorderSize(width_colorEditorBorder);
		this->colorEditor->setCornerSize(width_colorEditorCorner);
	}
}

void TrackView::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float width_head = this->sizes.width_trackHead * screenSize.getWidth();
	float height_borderTop = this->sizes.height_borderTop * screenSize.getHeight();
	float width_borderRight = this->sizes.width_borderRight * screenSize.getWidth();

	//计算轨道主体大小与曲线编辑器大小
	float height_track = this->getHeight();
	int curveSize = this->getCurveSize();
	if (this->isCurveShown()) {
		height_track = this->getHeight() / (1 + curveSize * this->scales.height_curveByTrack);
	}
	float height_curve = height_track * this->scales.height_curveByTrack;

	//绘高亮背景
	if (this->isSelected()) {
		//底层背景
		g.setColour(this->colors.background_trackView_highlight);
		g.fillAll();

		//轨道头背景
		juce::Rectangle<float> rectHead(0, 0, width_head, height_track);
		g.setColour(this->colors.background_trackViewHead_highlight);
		g.fillRect(rectHead);

		//轨道头右边框
		juce::Rectangle<float> rectHeadRightBorder(
			width_head - width_borderRight, 0, width_borderRight, height_track);
		g.setColour(this->colors.border);
		g.fillRect(rectHeadRightBorder);

		//上边框
		juce::Rectangle<float> rectTopBorder(
			0, 0, this->getWidth(), height_borderTop);
		g.setColour(this->colors.border);
		g.fillRect(rectTopBorder);
	}

	//绘曲线分割线
	if (this->isCurveShown()) {
		for (int i = 0; i < curveSize; i++) {
			juce::Rectangle<float> rectBorder(
				0, height_track + i * height_curve,
				this->getWidth(), height_borderTop
			);
			g.setColour(this->colors.border);
			g.fillRect(rectBorder);
		}
	}

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

void TrackView::mouseDown(const juce::MouseEvent& event)
{
	if (!this->getIsMaster()) {
		this->setCurrentTrackMethod(this->getIndex());
	}
}

void TrackView::mouseMove(const juce::MouseEvent& event)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float width_head = this->sizes.width_trackHead * screenSize.getWidth();

	float height_marginTop = this->sizes.height_trackHeadTopMargin * screenSize.getHeight();
	float height_marginBottom = this->sizes.height_trackHeadBottomMargin * screenSize.getHeight();
	float width_marginLeft = this->sizes.width_trackHeadLeftMargin * screenSize.getWidth();
	float width_marginRight = this->sizes.width_trackHeadRightMargin * screenSize.getWidth();

	float height_titleLine = this->sizes.height_trackHeadTitleAndButtonLine * screenSize.getHeight();

	float width_lightLeftMargin = this->sizes.width_trackHeadColorLightLeftMargin * screenSize.getWidth();
	float width_lightNameSplit = this->sizes.width_trackHeadLightNameSplit * screenSize.getWidth();
	float width_nameButtonSplit = this->sizes.width_trackHeadNameButtonSplit * screenSize.getWidth();
	float width_buttonSplit = this->sizes.width_trackHeadButtonSplit * screenSize.getWidth();

	float height_colorLight = this->scales.height_trackHeadColorLight * height_titleLine;
	float height_msButton = this->scales.height_trackHeadMSButton * height_titleLine;
	float width_colorLight = height_colorLight;
	float width_msButton = height_msButton;

	if (event.position.getX() < width_head) {
		//轨道头区域

		//判断鼠标位置
		juce::Rectangle<float> rectLightArea(
			width_marginLeft + width_lightLeftMargin + width_colorLight / 2,
			height_marginTop + height_titleLine / 2 - height_colorLight / 2,
			width_colorLight, height_colorLight
		);

		if (rectLightArea.contains(event.position)) {
			this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
		}
		else {
			this->setMouseCursor(juce::MouseCursor::NormalCursor);
		}
	}
	else {
		//TODO 轨道数据区
		this->setMouseCursor(juce::MouseCursor::NormalCursor);
	}
}

void TrackView::mouseDrag(const juce::MouseEvent& event)
{
	this->mouseMove(event);
}

void TrackView::mouseUp(const juce::MouseEvent& event)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float width_head = this->sizes.width_trackHead * screenSize.getWidth();

	float height_marginTop = this->sizes.height_trackHeadTopMargin * screenSize.getHeight();
	float height_marginBottom = this->sizes.height_trackHeadBottomMargin * screenSize.getHeight();
	float width_marginLeft = this->sizes.width_trackHeadLeftMargin * screenSize.getWidth();
	float width_marginRight = this->sizes.width_trackHeadRightMargin * screenSize.getWidth();

	float height_titleLine = this->sizes.height_trackHeadTitleAndButtonLine * screenSize.getHeight();

	float width_lightLeftMargin = this->sizes.width_trackHeadColorLightLeftMargin * screenSize.getWidth();
	float width_lightNameSplit = this->sizes.width_trackHeadLightNameSplit * screenSize.getWidth();
	float width_nameButtonSplit = this->sizes.width_trackHeadNameButtonSplit * screenSize.getWidth();
	float width_buttonSplit = this->sizes.width_trackHeadButtonSplit * screenSize.getWidth();

	float height_colorLight = this->scales.height_trackHeadColorLight * height_titleLine;
	float height_msButton = this->scales.height_trackHeadMSButton * height_titleLine;
	float width_colorLight = height_colorLight;
	float width_msButton = height_msButton;

	//左键
	if (event.mods.isLeftButtonDown()) {
		if (event.position.getX() < width_head) {
			//轨道头区域

			//判断鼠标位置
			juce::Rectangle<float> rectLightArea(
				width_marginLeft + width_lightLeftMargin + width_colorLight / 2,
				height_marginTop + height_titleLine / 2 - height_colorLight / 2,
				width_colorLight, height_colorLight
			);

			//更改颜色
			if (rectLightArea.contains(event.position)) {
				auto color = vocalshaper::TrackDAO::getColour(this->getTrack());
				auto type = vocalshaper::TrackDAO::getTrackType(this->getTrack());
				this->colorEditor->setCurrentColor(color);
				this->colorEditor->setSingerColor(
					type == vocalshaper::Track::TrackType::Voice &&
					!vocalshaper::TrackDAO::getSinger(this->getTrack()).isEmpty(),
					juce::Colour::fromRGB(98, 111, 252));//TODO 歌手颜色
				this->colorEditor->showAt(rectLightArea.toNearestIntEdges());
			}
		}
		else {
			//TODO 轨道数据区
		}
	}
}

void TrackView::mouseExit(const juce::MouseEvent& event)
{
	this->setMouseCursor(juce::MouseCursor::NormalCursor);
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
				if (target->track == -1) {
					track = vocalshaper::ProjectDAO::getMasterTrack(action.getProxy()->getPtr());
				}
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

void TrackView::listenNameChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	if (action.getBaseType() == vocalshaper::actions::ActionBase::Type::Track &&
		action.getActionType() == vocalshaper::actions::TrackAction::ActionType::Name) {
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
					auto name = vocalshaper::TrackDAO::getName(track);

					auto messageManager = juce::MessageManager::getInstance();
					if (!messageManager) {
						return;
					}

					messageManager->callAsync([this, name] {this->setTrackName(name); });
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

void TrackView::listenLinkChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	auto refreshFunc = [this] {
		juce::ScopedReadLock projLocker(this->getProjLock());
		if (this->getProject()) {
			juce::ScopedReadLock proxyLocker(this->getProject()->getLock());
			this->setTrackType(this->getTrack());
		}
	};

	if (action.getBaseType() == vocalshaper::actions::ActionBase::Type::Track &&
		(action.getActionType() == vocalshaper::actions::TrackAction::ActionType::AddNote ||
			action.getActionType() == vocalshaper::actions::TrackAction::ActionType::RemoveNote ||
			action.getActionType() == vocalshaper::actions::TrackAction::ActionType::AddWave ||
			action.getActionType() == vocalshaper::actions::TrackAction::ActionType::RemoveWave ||
			action.getActionType() == vocalshaper::actions::TrackAction::ActionType::AddInstr ||
			action.getActionType() == vocalshaper::actions::TrackAction::ActionType::RemoveInstr ||
			action.getActionType() == vocalshaper::actions::TrackAction::ActionType::Singer)) {
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
					auto messageManager = juce::MessageManager::getInstance();
					if (!messageManager) {
						return;
					}

					messageManager->callAsync(refreshFunc);
				}
			}
		}
	}
}

void TrackView::setColor(juce::Colour color)
{
	this->colorEditor->close();
	this->lafs.SMButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId,
		color
	);
	this->mButton->repaint();
	this->sButton->repaint();
	this->repaint();
}

void TrackView::setTrackName(const juce::String& name)
{
	this->nameEditor->setText(name);
}

void TrackView::setTrackType(const vocalshaper::Track* track)
{
	if (!track) {
		return;
	}

	switch (vocalshaper::TrackDAO::getTrackType(track))
	{
	case vocalshaper::Track::TrackType::Empty:
	{
		this->linkButton->setButtonText(this->tr("bt_EmptyTrack"));
		break;
	}
	case vocalshaper::Track::TrackType::Voice:
	{
		auto singer = vocalshaper::TrackDAO::getSinger(track);
		if (!singer.isEmpty()) {
			//TODO 将按钮设为显示名称
			this->linkButton->setButtonText(singer);
		}
		else {
			this->linkButton->setButtonText(this->tr("bt_UnlinkedVoiceTrack"));
		}
		break;
	}
	case vocalshaper::Track::TrackType::Midi:
	{
		auto instr = vocalshaper::TrackDAO::getInstrument(track);
		if (instr) {
			//TODO 将按钮设为显示名称
			this->linkButton->setButtonText(
				juce::String(vocalshaper::InstrDAO::getUniqueId(instr)));
		}
		else {
			this->linkButton->setButtonText(this->tr("bt_UnlinkedMidiTrack"));
		}
		break;
	}
	case vocalshaper::Track::TrackType::Wave:
	{
		this->linkButton->setButtonText(this->tr("bt_WaveTrack"));
		break;
	}
	}
}

void TrackView::sendSolo(bool solo)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		auto currentSolo = vocalshaper::TrackDAO::getSolo(this->getTrack());
		if (solo != currentSolo) {
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
}

void TrackView::sendMute(bool mute)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		auto currentMute = vocalshaper::TrackDAO::getMute(this->getTrack());
		if (mute != currentMute) {
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
}

void TrackView::sendName(const juce::String& name)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		auto currentName = vocalshaper::TrackDAO::getName(this->getTrack());
		if (name != currentName) {
			using ActionType = vocalshaper::actions::track::NameAction;
			ActionType::TargetType target{};
			target.track = this->getIndex();
			auto action = std::make_unique<ActionType>(
				target, name, this->getProject()
				);

			//发送事件
			this->getProject()->getProcesser()->processEvent(std::move(action));
		}
	}
}

void TrackView::sendColor(juce::Colour color)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		auto currentColor = vocalshaper::TrackDAO::getColour(this->getTrack());
		if (color != currentColor) {
			using ActionType = vocalshaper::actions::track::ColourAction;
			ActionType::TargetType target{};
			target.track = this->getIndex();
			auto action = std::make_unique<ActionType>(
				target, color, this->getProject()
				);

			//发送事件
			this->getProject()->getProcesser()->processEvent(std::move(action));
		}
	}
}

void TrackView::showLinkMenu()
{
	//TODO 显示链接列表（声库、乐器）
}

void TrackView::showCurve(bool show)
{
	if (this->showCurveMethod) {
		this->showCurveMethod(this->getTrack(), show);
	}
}

void TrackView::initUIConfigAndIcon()
{
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border", this->colors.border, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-trackNameEditor", this->colors.text_trackNameEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-trackNameEditor-empty", this->colors.text_trackNameEditor_empty, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-trackNameEditor", this->colors.background_trackNameEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-trackNameEditor-highlight", this->colors.text_trackNameEditor_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-trackNameEditor-highlight", this->colors.background_trackNameEditor_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "caret-trackNameEditor", this->colors.caret_trackNameEditor, result
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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-trackViewLinkButton", this->colors.text_trackViewLinkButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-trackViewLinkButton", this->colors.background_trackViewLinkButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-trackViewShowCurveButton", this->colors.icon_trackViewShowCurveButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-trackViewShowCurveButton", this->colors.background_trackViewShowCurveButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-trackViewShowCurveButton-highlight", this->colors.icon_trackViewShowCurveButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-trackViewShowCurveButton-highlight", this->colors.background_trackViewShowCurveButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-trackView-highlight", this->colors.background_trackView_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-trackViewHead-highlight", this->colors.background_trackViewHead_highlight, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-trackHead", this->sizes.width_trackHead, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-borderTop", this->sizes.height_borderTop, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-borderRight", this->sizes.width_borderRight, result
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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditor", this->sizes.width_colorEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-colorEditor", this->sizes.height_colorEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorCalloutArrow", this->sizes.width_colorEditorCalloutArrow, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorCalloutBorder", this->sizes.width_colorEditorCalloutBorder, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorCalloutCorner", this->sizes.width_colorEditorCalloutCorner, result
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
	juce::String iconShowCurveButton;
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-showCurveButton", iconShowCurveButton, result
		);

	//加载显示曲线按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconShowCurveButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconCurve = juce::Drawable::createFromSVG(*ptrXml);
				this->iconCurveHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconCurve) {
					this->iconCurve->replaceColour(
						juce::Colours::black, this->colors.icon_trackViewShowCurveButton
					);
				}
				if (this->iconCurveHighlight) {
					this->iconCurveHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_trackViewShowCurveButton_highlight
					);
				}
			}
		}
	}
}