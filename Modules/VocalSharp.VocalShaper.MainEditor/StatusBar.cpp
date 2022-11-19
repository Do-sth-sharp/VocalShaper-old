#include "StatusBar.h"
#include <libJModule.h>
#include "StatusBarFlatTextButtonLAF.h"
#include "StatusBarRoundButttonLAF.h"

StatusBar::StatusBar()
	: EditorBase()
{
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

	//以下获取命令管理器
	jmadf::CallInterface<juce::ApplicationCommandManager*&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandManager",
		this->commandManager
		);

	//以下获取命令ID
	this->initCommandID();

	//以下注册命令回调
	this->initCommandFunction();

	//以下注册Flag获取函数
	this->initCommandFlagHook();

	//以下获取界面属性
	this->initUIConfigAndIcon();

	//建立文字按钮样式
	this->lafs.statusTextButton = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.statusTextButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_statusBarTextButton
	);
	this->lafs.statusTextButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_statusBarTextButton_highlight
	);
	this->lafs.statusTextButton->setColour(
		juce::TextButton::ColourIds::textColourOnId, this->colors.text_statusBarTextButton_highlight
	);
	this->lafs.statusTextButton->setColour(
		juce::TextButton::ColourIds::textColourOffId, this->colors.text_statusBarTextButton
	);
	this->lafs.statusTextButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//建立按钮样式
	this->lafs.statusButton = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.statusButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_statusBarButton
	);
	this->lafs.statusButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_statusBarButton_highlight
	);
	this->lafs.statusButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//建立编辑器开关按钮
	this->mixtureEditorButton = std::make_unique<juce::TextButton>(
		this->tr("bt_MixtureEditor"), this->tr("tip_MixtureEditor"));
	this->mixtureEditorButton->setCommandToTrigger(this->commandManager, this->showMixtureEditorCommandID, true);
	this->mixtureEditorButton->setWantsKeyboardFocus(false);
	this->mixtureEditorButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->mixtureEditorButton->setLookAndFeel(this->lafs.statusTextButton.get());
	this->addAndMakeVisible(this->mixtureEditorButton.get());

	this->additionEditorButton = std::make_unique<juce::TextButton>(
		this->tr("bt_AdditionEditor"), this->tr("tip_AdditionEditor"));
	this->additionEditorButton->setCommandToTrigger(this->commandManager, this->showAdditionEditorCommandID, true);
	this->additionEditorButton->setWantsKeyboardFocus(false);
	this->additionEditorButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->additionEditorButton->setLookAndFeel(this->lafs.statusTextButton.get());
	this->addAndMakeVisible(this->additionEditorButton.get());

	//建立编辑器类型按钮
	this->noteEditorButton = std::make_unique<juce::DrawableButton>(
		"bt_NoteEditor", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->noteEditorButton->setImages(this->iconNoteEditor.get(), nullptr, nullptr, nullptr,
		this->iconNoteEditorHighlight.get(), nullptr, nullptr, nullptr);
	this->noteEditorButton->setLookAndFeel(this->lafs.statusButton.get());
	this->noteEditorButton->setWantsKeyboardFocus(false);
	this->noteEditorButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->noteEditorButton->setCommandToTrigger(this->commandManager, this->noteEditorCommandID, true);
	this->addAndMakeVisible(this->noteEditorButton.get());

	this->mixEditorButton = std::make_unique<juce::DrawableButton>(
		"bt_MixEditor", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->mixEditorButton->setImages(this->iconMixEditor.get(), nullptr, nullptr, nullptr,
		this->iconMixEditorHighlight.get(), nullptr, nullptr, nullptr);
	this->mixEditorButton->setLookAndFeel(this->lafs.statusButton.get());
	this->mixEditorButton->setWantsKeyboardFocus(false);
	this->mixEditorButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->mixEditorButton->setCommandToTrigger(this->commandManager, this->mixEditorCommandID, true);
	this->addAndMakeVisible(this->mixEditorButton.get());

	this->scriptEditorButton = std::make_unique<juce::DrawableButton>(
		"bt_ScriptEditor", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->scriptEditorButton->setImages(this->iconScriptEditor.get(), nullptr, nullptr, nullptr,
		this->iconScriptEditorHighlight.get(), nullptr, nullptr, nullptr);
	this->scriptEditorButton->setLookAndFeel(this->lafs.statusButton.get());
	this->scriptEditorButton->setWantsKeyboardFocus(false);
	this->scriptEditorButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->scriptEditorButton->setCommandToTrigger(this->commandManager, this->scriptEditorCommandID, true);
	this->addAndMakeVisible(this->scriptEditorButton.get());

	//建立编辑器插件按钮
	this->noteEditorPluginButton = std::make_unique<juce::TextButton>(
		this->tr("bt_NoteEditorPlugin"), this->tr("tip_NoteEditorPlugin"));
	this->noteEditorPluginButton->setCommandToTrigger(this->commandManager, this->noteEditorPluginCommandID, true);
	this->noteEditorPluginButton->setWantsKeyboardFocus(false);
	this->noteEditorPluginButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->noteEditorPluginButton->setLookAndFeel(this->lafs.statusTextButton.get());
	this->addAndMakeVisible(this->noteEditorPluginButton.get());

	this->noteEditorAdditionPluginButton = std::make_unique<juce::TextButton>(
		this->tr("bt_NoteEditorAdditionPlugin"), this->tr("tip_NoteEditorAdditionPlugin"));
	this->noteEditorAdditionPluginButton->setCommandToTrigger(this->commandManager, this->noteEditorAdditionPluginCommandID, true);
	this->noteEditorAdditionPluginButton->setWantsKeyboardFocus(false);
	this->noteEditorAdditionPluginButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->noteEditorAdditionPluginButton->setLookAndFeel(this->lafs.statusTextButton.get());
	this->addAndMakeVisible(this->noteEditorAdditionPluginButton.get());

	//获取翻页按钮样式
	this->lafs.statusRoundButton = std::unique_ptr<juce::LookAndFeel>(new StatusBarRoundButtonLAF);
	this->lafs.statusRoundButton->setColour(
		juce::DrawableButton::ColourIds::backgroundColourId, this->colors.background_statusBarRoundButton
	);
	this->lafs.statusRoundButton->setColour(
		juce::DrawableButton::ColourIds::backgroundOnColourId, this->colors.background_statusBarRoundButton
	);
	this->lafs.statusRoundButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//建立翻页按钮
	this->lastTrackButton = std::make_unique<juce::DrawableButton>(
		"bt_LastTrack", juce::DrawableButton::ButtonStyle::ImageFitted);
	this->lastTrackButton->setImages(this->iconLastTrack.get());
	this->lastTrackButton->setLookAndFeel(this->lafs.statusRoundButton.get());
	this->lastTrackButton->setWantsKeyboardFocus(false);
	this->lastTrackButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->lastTrackButton->setCommandToTrigger(this->commandManager, this->lastTrackCommandID, true);
	this->addAndMakeVisible(this->lastTrackButton.get());

	this->nextTrackButton = std::make_unique<juce::DrawableButton>(
		"bt_NextTrack", juce::DrawableButton::ButtonStyle::ImageFitted);
	this->nextTrackButton->setImages(this->iconNextTrack.get());
	this->nextTrackButton->setLookAndFeel(this->lafs.statusRoundButton.get());
	this->nextTrackButton->setWantsKeyboardFocus(false);
	this->nextTrackButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->nextTrackButton->setCommandToTrigger(this->commandManager, this->nextTrackCommandID, true);
	this->addAndMakeVisible(this->nextTrackButton.get());

	//获取轨道切换按钮样式
	this->lafs.statusSwitchTrackButton = std::unique_ptr<juce::LookAndFeel>(new StatusBarFlatTextButtonLAF);
	this->lafs.statusSwitchTrackButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_statusBar
	);
	this->lafs.statusSwitchTrackButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_statusBar
	);
	this->lafs.statusSwitchTrackButton->setColour(
		juce::TextButton::ColourIds::textColourOnId, this->colors.text_statusBarCurrentTrackName
	);
	this->lafs.statusSwitchTrackButton->setColour(
		juce::TextButton::ColourIds::textColourOffId, this->colors.text_statusBarCurrentTrackName
	);
	this->lafs.statusSwitchTrackButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//建立轨道切换按钮
	this->switchTrackButton = std::make_unique<juce::TextButton>(
		juce::String(), this->tr("tip_Track"));
	this->switchTrackButton->setConnectedEdges(juce::Button::ConnectedEdgeFlags::ConnectedOnLeft |
		juce::Button::ConnectedEdgeFlags::ConnectedOnRight);
	this->switchTrackButton->setWantsKeyboardFocus(false);
	this->switchTrackButton->setFocusContainerType(juce::Component::FocusContainerType::none);
	this->switchTrackButton->setLookAndFeel(this->lafs.statusSwitchTrackButton.get());
	this->switchTrackButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->switchTrackButton->setCommandToTrigger(this->commandManager, this->switchTrackCommandID, false);
	this->addAndMakeVisible(this->switchTrackButton.get());

	//刷新轨道状态
	this->refreshTrackComponent();
}

void StatusBar::setProjectCallback(const vocalshaper::ProjectProxy* ptr)
{
	this->refreshTrackComponent();
}

void StatusBar::setTrackIDCallback(int trackID)
{
	this->refreshTrackComponent();
}

void StatusBar::editorControlerChanged(juce::Component* controler)
{
	juce::ScopedWriteLock locker(this->controlerLock);
	if (this->editorControlerComponent) {
		this->removeChildComponent(this->editorControlerComponent);
	}
	this->editorControlerComponent = controler;
	if (controler) {
		this->addChildComponent(controler);
	}
	this->resized();
	this->repaint();
}

int StatusBar::selectNoteEditorPlugin(const juce::Array<std::pair<juce::String, bool>>& list)
{
	juce::PopupMenu menu;
	for (int i = 0; i < list.size(); i++) {
		auto& pair = list.getReference(i);
		menu.addItem(i + 1, pair.first, true, pair.second);
	}
	return menu.showAt(this->noteEditorPluginButton.get(), 0, 0, 10);
}

int StatusBar::selectNoteEditionAdditionPlugin(const juce::Array<std::pair<juce::String, bool>>& list)
{
	juce::PopupMenu menu;
	for (int i = 0; i < list.size(); i++) {
		auto& pair = list.getReference(i);
		menu.addItem(i + 1, pair.first, true, pair.second);
	}
	return menu.showAt(this->noteEditorAdditionPluginButton.get(), 0, 0, 10);
}

int StatusBar::switchTrack()
{
	juce::ScopedReadLock locker1(this->getProjLock());
	juce::PopupMenu menu;

	if (this->getProject()) {
		juce::String prefix = this->tr("pre_Track");
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		for (int i = 0; i < vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr()); i++) {
			auto* track = vocalshaper::ProjectDAO::getTrack(this->getProject()->getPtr(), i);
			juce::String name = prefix + " " + juce::String(i);
			juce::Colour color = vocalshaper::TrackDAO::getColour(track);

			menu.addColouredItem(i + 1, name, this->colors.text_statusBarCurrentTrackName,
				true, i == this->getTrackID(), nullptr);
		}
	}
	return menu.showAt(this->switchTrackButton.get(), 0, 0, 10);
}

void StatusBar::resized()
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算右侧大小
	int width_borderRight = this->sizes.width_statusBorderRight * screenSize.getWidth();
	int width_marginRight = this->sizes.width_statusMarginRight * screenSize.getWidth();
	int width_textButtonMargin = this->sizes.width_statusButtonMargin * screenSize.getWidth();
	int height_button = this->scales.height_statusButton * this->getHeight();
	int width_button = height_button;
	int width_buttonSplit = this->sizes.width_statusButtonSplit * screenSize.getWidth();
	int width_buttonGroupSplit = this->sizes.width_statusButtonGroupSplit * screenSize.getWidth();
	int width_splitLine = this->sizes.width_statusSplitLine * screenSize.getWidth();
	int height_splitLine = this->scales.height_statusSplitLine * this->getHeight();

	//计算文本按钮宽度
	juce::Font font =
		juce::LookAndFeel::getDefaultLookAndFeel().getTextButtonFont(*(this->mixtureEditorButton.get()), height_button);
	int width_mixtureEditorButton = 
		font.getStringWidth(this->mixEditorButton->getButtonText()) + width_textButtonMargin * 2;
	int width_additionEditorButton = 
		font.getStringWidth(this->additionEditorButton->getButtonText()) + width_textButtonMargin * 2;
	int width_noteEditorPluginButton = 
		font.getStringWidth(this->noteEditorPluginButton->getButtonText()) + width_textButtonMargin * 2;
	int width_noteEditorAdditionPluginButton = 
		font.getStringWidth(this->noteEditorAdditionPluginButton->getButtonText()) + width_textButtonMargin * 2;

	//调整编辑器开关按钮位置
	this->mixtureEditorButton->setBounds(
		this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 0 - width_buttonGroupSplit * 0 - width_button * 0
		- width_mixtureEditorButton,
		this->getHeight() / 2 - height_button / 2,
		width_mixtureEditorButton, height_button
	);
	this->additionEditorButton->setBounds(
		this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 1 - width_buttonGroupSplit * 0 - width_button * 0
		- width_mixtureEditorButton - width_additionEditorButton,
		this->getHeight() / 2 - height_button / 2,
		width_additionEditorButton, height_button
	);

	//调整编辑器类型按钮位置
	this->scriptEditorButton->setBounds(
		this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 1 - width_buttonGroupSplit * 1 - width_button * 1
		- width_mixtureEditorButton - width_additionEditorButton,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);
	this->mixEditorButton->setBounds(
		this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 2 - width_buttonGroupSplit * 1 - width_button * 2
		- width_mixtureEditorButton - width_additionEditorButton,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);
	this->noteEditorButton->setBounds(
		this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 3 - width_buttonGroupSplit * 1 - width_button * 3
		- width_mixtureEditorButton - width_additionEditorButton,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);

	//调整编辑器插件按钮
	this->noteEditorPluginButton->setBounds(
		this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 3 - width_buttonGroupSplit * 2 - width_button * 3
		- width_mixtureEditorButton - width_additionEditorButton
		- width_noteEditorPluginButton,
		this->getHeight() / 2 - height_button / 2,
		width_noteEditorPluginButton, height_button
	);
	this->noteEditorAdditionPluginButton->setBounds(
		this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 4 - width_buttonGroupSplit * 2 - width_button * 3
		- width_mixtureEditorButton - width_additionEditorButton
		- width_noteEditorPluginButton - width_noteEditorAdditionPluginButton,
		this->getHeight() / 2 - height_button / 2,
		width_noteEditorAdditionPluginButton, height_button
	);

	//计算左侧大小
	int width_marginLeft = this->sizes.width_statusMarginLeft * screenSize.getWidth();
	int width_trackSplit = this->sizes.width_statusTrackComponentSplit * screenSize.getWidth();
	int width_trackTextMargin = this->sizes.width_statusTrackComponentTextButtonMargin * screenSize.getWidth();
	int height_trackButton = this->scales.height_statusRoundButton * this->getHeight();
	int width_trackButton = height_trackButton;
	int height_trackColor = this->scales.height_statusTrackColorPoint * this->getHeight();
	int width_trackColor = height_trackColor;

	//计算轨道切换按钮宽度
	juce::Font switchFont =
		juce::LookAndFeel::getDefaultLookAndFeel().getTextButtonFont(*(this->switchTrackButton.get()), height_button);
	int width_switchTrackButton =
		switchFont.getStringWidth(this->switchTrackButton->getButtonText()) + width_trackTextMargin * 2;

	//根据剩余空间计算控件
	int leftWidth = this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 4 - width_buttonGroupSplit * 3 - width_button * 3
		- width_mixtureEditorButton - width_additionEditorButton
		- width_noteEditorPluginButton - width_noteEditorAdditionPluginButton;
	int allWidth = width_marginLeft + width_trackButton * 2 + width_trackSplit * 3 + width_trackColor + width_switchTrackButton + width_buttonGroupSplit;
	int fullTrackWidth = allWidth - width_buttonGroupSplit;
	int textTrackWidth = fullTrackWidth - width_trackColor - width_trackSplit;
	int lightTrackWidth = fullTrackWidth - width_switchTrackButton - width_trackSplit;
	int buttonTrackWidth = fullTrackWidth - width_trackColor - width_switchTrackButton - width_trackSplit * 2;

	//调整左侧控件
	juce::ScopedReadLock locker1(this->controlerLock);
	if (leftWidth > allWidth) {
		//显示完整轨道控件和控制器
		this->lastTrackButton->setBounds(
			width_marginLeft, this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->lastTrackButton->setVisible(true);

		this->switchTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit * 2 + width_trackColor,
			this->getHeight() / 2 - height_button / 2,
			width_switchTrackButton, height_button
		);
		this->switchTrackButton->setVisible(true);

		this->nextTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit * 3 + width_trackColor + width_switchTrackButton,
			this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->nextTrackButton->setVisible(true);

		if (this->editorControlerComponent) {
			this->editorControlerComponent->setBounds(
				allWidth, 0,
				leftWidth - allWidth, this->getHeight()
			);
			this->editorControlerComponent->setVisible(true);
		}
	}
	else if (leftWidth >= fullTrackWidth) {
		//只显示完整轨道控件
		this->lastTrackButton->setBounds(
			width_marginLeft, this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->lastTrackButton->setVisible(true);

		this->switchTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit * 2 + width_trackColor,
			this->getHeight() / 2 - height_button / 2,
			width_switchTrackButton, height_button
		);
		this->switchTrackButton->setVisible(true);

		this->nextTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit * 3 + width_trackColor + width_switchTrackButton,
			this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->nextTrackButton->setVisible(true);

		if (this->editorControlerComponent) {
			this->editorControlerComponent->setBounds(
				allWidth, 0,
				0, this->getHeight()
			);
			this->editorControlerComponent->setVisible(false);
		}
	}
	else if (leftWidth >= textTrackWidth) {
		//显示文字版轨道控件
		this->lastTrackButton->setBounds(
			width_marginLeft, this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->lastTrackButton->setVisible(true);

		this->switchTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit,
			this->getHeight() / 2 - height_button / 2,
			width_switchTrackButton, height_button
		);
		this->switchTrackButton->setVisible(true);

		this->nextTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit * 2 + width_switchTrackButton,
			this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->nextTrackButton->setVisible(true);

		if (this->editorControlerComponent) {
			this->editorControlerComponent->setBounds(
				allWidth, 0,
				0, this->getHeight()
			);
			this->editorControlerComponent->setVisible(false);
		}
	}
	else if (leftWidth >= lightTrackWidth) {
		//显示颜色版轨道控件
		this->lastTrackButton->setBounds(
			width_marginLeft, this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->lastTrackButton->setVisible(true);

		this->switchTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit,
			this->getHeight() / 2 - height_button / 2,
			width_switchTrackButton, height_button
		);
		this->switchTrackButton->setVisible(false);

		this->nextTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit * 2 + width_trackColor,
			this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->nextTrackButton->setVisible(true);

		if (this->editorControlerComponent) {
			this->editorControlerComponent->setBounds(
				allWidth, 0,
				0, this->getHeight()
			);
			this->editorControlerComponent->setVisible(false);
		}
	}
	else if (leftWidth >= buttonTrackWidth) {
		//只显示轨道切换按钮
		this->lastTrackButton->setBounds(
			width_marginLeft, this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->lastTrackButton->setVisible(true);

		this->switchTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit,
			this->getHeight() / 2 - height_button / 2,
			width_switchTrackButton, height_button
		);
		this->switchTrackButton->setVisible(false);

		this->nextTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit,
			this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->nextTrackButton->setVisible(true);

		if (this->editorControlerComponent) {
			this->editorControlerComponent->setBounds(
				allWidth, 0,
				0, this->getHeight()
			);
			this->editorControlerComponent->setVisible(false);
		}
	}
	else {
		//不显示左侧控件
		this->lastTrackButton->setBounds(
			width_marginLeft, this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->lastTrackButton->setVisible(false);

		this->switchTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit + width_trackColor,
			this->getHeight() / 2 - height_button / 2,
			width_switchTrackButton, height_button
		);
		this->switchTrackButton->setVisible(false);

		this->nextTrackButton->setBounds(
			width_marginLeft + width_trackButton + width_trackSplit * 2 + width_trackColor + width_switchTrackButton,
			this->getHeight() / 2 - height_trackButton / 2,
			width_trackButton, height_trackButton
		);
		this->nextTrackButton->setVisible(false);

		if (this->editorControlerComponent) {
			this->editorControlerComponent->setBounds(
				allWidth, 0,
				0, this->getHeight()
			);
			this->editorControlerComponent->setVisible(false);
		}
	}
}

void StatusBar::paint(juce::Graphics& g)
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//填充背景
	g.fillAll(this->colors.background_statusBar);

	//计算并绘制边框
	int width_borderRight = this->sizes.width_statusBorderRight * screenSize.getWidth();
	juce::Rectangle<int> rectTopBorder(
		this->getWidth() - width_borderRight, 0,
		width_borderRight, this->getHeight()
	);
	g.setColour(this->colors.border_statusBar);
	g.fillRect(rectTopBorder);

	//计算右侧大小
	int width_marginRight = this->sizes.width_statusMarginRight * screenSize.getWidth();
	int width_textButtonMargin = this->sizes.width_statusButtonMargin * screenSize.getWidth();
	int height_button = this->scales.height_statusButton * this->getHeight();
	int width_button = height_button;
	int width_buttonSplit = this->sizes.width_statusButtonSplit * screenSize.getWidth();
	int width_buttonGroupSplit = this->sizes.width_statusButtonGroupSplit * screenSize.getWidth();
	int width_splitLine = this->sizes.width_statusSplitLine * screenSize.getWidth();
	int height_splitLine = this->scales.height_statusSplitLine * this->getHeight();

	//计算文本按钮宽度
	juce::Font font =
		juce::LookAndFeel::getDefaultLookAndFeel().getTextButtonFont(*(this->mixtureEditorButton.get()), height_button);
	int width_mixtureEditorButton =
		font.getStringWidth(this->mixEditorButton->getButtonText()) + width_textButtonMargin * 2;
	int width_additionEditorButton =
		font.getStringWidth(this->additionEditorButton->getButtonText()) + width_textButtonMargin * 2;
	int width_noteEditorPluginButton =
		font.getStringWidth(this->noteEditorPluginButton->getButtonText()) + width_textButtonMargin * 2;
	int width_noteEditorAdditionPluginButton =
		font.getStringWidth(this->noteEditorAdditionPluginButton->getButtonText()) + width_textButtonMargin * 2;

	//计算分割线位置
	int posXLineR1 = this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 1 - width_buttonGroupSplit * 0 - width_button * 0
		- width_mixtureEditorButton - width_additionEditorButton
		- width_buttonGroupSplit / 2;
	int posXLineR2 = this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 3 - width_buttonGroupSplit * 1 - width_button * 3
		- width_mixtureEditorButton - width_additionEditorButton
		- width_buttonGroupSplit / 2;
	int posXLineR3 = this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 4 - width_buttonGroupSplit * 2 - width_button * 3
		- width_mixtureEditorButton - width_additionEditorButton
		- width_noteEditorPluginButton - width_noteEditorAdditionPluginButton
		- width_buttonGroupSplit / 2;

	//绘制分割线
	g.setColour(this->colors.split_statusBar);
	g.drawLine(
		posXLineR1, this->getHeight() / 2.f - height_splitLine / 2.f,
		posXLineR1, this->getHeight() / 2.f + height_splitLine / 2.f,
		width_splitLine
	);
	g.drawLine(
		posXLineR2, this->getHeight() / 2.f - height_splitLine / 2.f,
		posXLineR2, this->getHeight() / 2.f + height_splitLine / 2.f,
		width_splitLine
	);
	g.drawLine(
		posXLineR3, this->getHeight() / 2.f - height_splitLine / 2.f,
		posXLineR3, this->getHeight() / 2.f + height_splitLine / 2.f,
		width_splitLine
	);

	//计算左侧大小
	int width_marginLeft = this->sizes.width_statusMarginLeft * screenSize.getWidth();
	int width_trackSplit = this->sizes.width_statusTrackComponentSplit * screenSize.getWidth();
	int width_trackTextMargin = this->sizes.width_statusTrackComponentTextButtonMargin * screenSize.getWidth();
	int height_trackButton = this->scales.height_statusRoundButton * this->getHeight();
	int width_trackButton = height_trackButton;
	int height_trackColor = this->scales.height_statusTrackColorPoint * this->getHeight();
	int width_trackColor = height_trackColor;

	//计算轨道切换按钮宽度
	juce::Font switchFont =
		juce::LookAndFeel::getDefaultLookAndFeel().getTextButtonFont(*(this->switchTrackButton.get()), height_button);
	int width_switchTrackButton =
		switchFont.getStringWidth(this->switchTrackButton->getButtonText()) + width_trackTextMargin * 2;

	//根据剩余空间计算控件
	int leftWidth = this->getWidth() - width_borderRight - width_marginRight
		- width_buttonSplit * 4 - width_buttonGroupSplit * 3 - width_button * 3
		- width_mixtureEditorButton - width_additionEditorButton
		- width_noteEditorPluginButton - width_noteEditorAdditionPluginButton;
	int allWidth = width_marginLeft + width_trackButton * 2 + width_trackSplit * 3 + width_trackColor + width_switchTrackButton + width_buttonGroupSplit;
	int fullTrackWidth = allWidth - width_buttonGroupSplit;
	int textTrackWidth = fullTrackWidth - width_trackColor - width_trackSplit;
	int lightTrackWidth = fullTrackWidth - width_switchTrackButton - width_trackSplit;
	int buttonTrackWidth = fullTrackWidth - width_trackColor - width_switchTrackButton - width_trackSplit * 2;

	//调整左侧控件
	juce::ScopedReadLock locker1(this->controlerLock);
	if (leftWidth > allWidth) {
		//显示完整轨道控件和控制器
		
		//绘分割线
		int posXLineL1 = allWidth - width_buttonGroupSplit / 2;
		g.setColour(this->colors.split_statusBar);
		g.drawLine(
			posXLineL1, this->getHeight() / 2.f - height_splitLine / 2.f,
			posXLineL1, this->getHeight() / 2.f + height_splitLine / 2.f,
			width_splitLine
		);

		//绘轨道颜色
		g.setColour(this->getCurrentTrackColour());
		juce::Rectangle<float> colorRect(
			width_marginLeft + width_trackButton + width_trackSplit,
			this->getHeight() / 2.f - height_trackColor / 2.f,
			width_trackColor, height_trackColor
		);
		g.fillEllipse(colorRect);
	}
	else if (leftWidth >= fullTrackWidth) {
		//只显示完整轨道控件
		
		//绘轨道颜色
		g.setColour(this->getCurrentTrackColour());
		juce::Rectangle<float> colorRect(
			width_marginLeft + width_trackButton + width_trackSplit,
			this->getHeight() / 2.f - height_trackColor / 2.f,
			width_trackColor, height_trackColor
		);
		g.fillEllipse(colorRect);
	}
	else if (leftWidth >= textTrackWidth) {
		//显示文字版轨道控件
	}
	else if (leftWidth >= lightTrackWidth) {
		//显示颜色版轨道控件
		
		//绘轨道颜色
		g.setColour(this->getCurrentTrackColour());
		juce::Rectangle<float> colorRect(
			width_marginLeft + width_trackButton + width_trackSplit,
			this->getHeight() / 2.f - height_trackColor / 2.f,
			width_trackColor, height_trackColor
		);
		g.fillEllipse(colorRect);
	}
	else if (leftWidth >= buttonTrackWidth) {
		//只显示轨道切换按钮
	}
	else {
		//不显示左侧控件
	}
}

void StatusBar::initCommandID()
{
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Show Mixture Editor", this->showMixtureEditorCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Show Addition Editor", this->showAdditionEditorCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Note Editor", this->noteEditorCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Mix Editor", this->mixEditorCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Script Editor", this->scriptEditorCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Note Editor Plugin", this->noteEditorPluginCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Note Editor Addition Plugin", this->noteEditorAdditionPluginCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Last Track", this->lastTrackCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Next Track", this->nextTrackCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Switch Track", this->switchTrackCommandID
		);
}

void StatusBar::initCommandFunction()
{
	
}

void StatusBar::initCommandFlagHook()
{

}

void StatusBar::initUIConfigAndIcon()
{
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-statusBar", this->colors.background_statusBar, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border-statusBar", this->colors.border_statusBar, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-statusBarTextButton", this->colors.text_statusBarTextButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-statusBarTextButton", this->colors.background_statusBarTextButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-statusBarTextButton-highlight", this->colors.text_statusBarTextButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-statusBarTextButton-highlight", this->colors.background_statusBarTextButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-statusBarButton", this->colors.icon_statusBarButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-statusBarButton", this->colors.background_statusBarButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-statusBarButton-highlight", this->colors.icon_statusBarButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-statusBarButton-highlight", this->colors.background_statusBarButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "split-statusBar", this->colors.split_statusBar, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-statusBarRoundButton", this->colors.icon_statusBarRoundButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-statusBarRoundButton", this->colors.background_statusBarRoundButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-statusBarCurrentTrackName", this->colors.text_statusBarCurrentTrackName, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "point-statusBarCurrentTrackDefault", this->colors.point_statusBarCurrentTrackDefault, result
		);
	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusBorderRight", this->sizes.width_statusBorderRight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusMarginRight", this->sizes.width_statusMarginRight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusButtonSplit", this->sizes.width_statusButtonSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusButtonGroupSplit", this->sizes.width_statusButtonGroupSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusSplitLine", this->sizes.width_statusSplitLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusButtonMargin", this->sizes.width_statusButtonMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusMarginLeft", this->sizes.width_statusMarginLeft, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusTrackComponentSplit", this->sizes.width_statusTrackComponentSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-statusTrackComponentTextButtonMargin", this->sizes.width_statusTrackComponentTextButtonMargin, result
		);
	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-statusButton", this->scales.height_statusButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-statusSplitLine", this->scales.height_statusSplitLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-statusRoundButton", this->scales.height_statusRoundButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-statusTrackColorPoint", this->scales.height_statusTrackColorPoint, result
		);
	//resource
	juce::String iconNoteEditorButton, iconMixEditorButton, iconScriptEditorButton;
	juce::String iconLastTrackButton, iconNextTrackButton;
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-noteEditorButton", iconNoteEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-mixEditorButton", iconMixEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-scriptEditorButton", iconScriptEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-lastTrackButton", iconLastTrackButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-nextTrackButton", iconNextTrackButton, result
		);

	//加载音符编辑器按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconNoteEditorButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconNoteEditor = juce::Drawable::createFromSVG(*ptrXml);
				this->iconNoteEditorHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconNoteEditor) {
					this->iconNoteEditor->replaceColour(
						juce::Colours::black, this->colors.icon_statusBarButton
					);
				}
				if (this->iconNoteEditorHighlight) {
					this->iconNoteEditorHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_statusBarButton_highlight
					);
				}
			}
		}
	}

	//加载混音器按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconMixEditorButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconMixEditor = juce::Drawable::createFromSVG(*ptrXml);
				this->iconMixEditorHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconMixEditor) {
					this->iconMixEditor->replaceColour(
						juce::Colours::black, this->colors.icon_statusBarButton
					);
				}
				if (this->iconMixEditorHighlight) {
					this->iconMixEditorHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_statusBarButton_highlight
					);
				}
			}
		}
	}

	//加载脚本编辑器按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconScriptEditorButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconScriptEditor = juce::Drawable::createFromSVG(*ptrXml);
				this->iconScriptEditorHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconScriptEditor) {
					this->iconScriptEditor->replaceColour(
						juce::Colours::black, this->colors.icon_statusBarButton
					);
				}
				if (this->iconScriptEditorHighlight) {
					this->iconScriptEditorHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_statusBarButton_highlight
					);
				}
			}
		}
	}

	//加载上一轨按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconLastTrackButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconLastTrack = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconLastTrack) {
					this->iconLastTrack->replaceColour(
						juce::Colours::black, this->colors.icon_statusBarRoundButton
					);
				}
			}
		}
	}

	//加载下一轨按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconNextTrackButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconNextTrack = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconNextTrack) {
					this->iconNextTrack->replaceColour(
						juce::Colours::black, this->colors.icon_statusBarRoundButton
					);
				}
			}
		}
	}
}

void StatusBar::refreshTrackComponent()
{
	juce::ScopedReadLock locker1(this->getProjLock());
	juce::String buttonText = this->tr("bt_NoTrack");

	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		if (this->getTrackID() >= 0 &&
			this->getTrackID() < ::vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr())) {
			buttonText = this->tr("pre_Track") + " " + juce::String(this->getTrackID());
		}
	}

	this->switchTrackButton->setButtonText(buttonText);
	this->resized();
	this->repaint();
}

juce::Colour StatusBar::getCurrentTrackColour()
{
	juce::ScopedReadLock locker1(this->getProjLock());
	juce::Colour result = this->colors.point_statusBarCurrentTrackDefault;

	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		if (this->getTrackID() >= 0 &&
			this->getTrackID() < ::vocalshaper::ProjectDAO::trackSize(
				this->getProject()->getPtr())) {
			auto* track = vocalshaper::ProjectDAO::getTrack(
				this->getProject()->getPtr(), this->getTrackID());
			result = vocalshaper::TrackDAO::getColour(track);
		}
	}

	return result;
}
