#include "StatusBar.h"
#include <libJModule.h>

StatusBar::StatusBar()
	:Component("Status Bar")
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
	//resource
	juce::String iconNoteEditorButton, iconMixEditorButton, iconScriptEditorButton;
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

	//建立文字按钮样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStatusTextButtonLAF",
		this->lafs.statusTextButton
		);
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
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStatusButtonLAF",
		this->lafs.statusButton
		);
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
	this->mixtureEditorButton->setLookAndFeel(this->lafs.statusTextButton);
	this->addAndMakeVisible(this->mixtureEditorButton.get());

	this->additionEditorButton = std::make_unique<juce::TextButton>(
		this->tr("bt_AdditionEditor"), this->tr("tip_AdditionEditor"));
	this->additionEditorButton->setCommandToTrigger(this->commandManager, this->showAdditionEditorCommandID, true);
	this->additionEditorButton->setWantsKeyboardFocus(false);
	this->additionEditorButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->additionEditorButton->setLookAndFeel(this->lafs.statusTextButton);
	this->addAndMakeVisible(this->additionEditorButton.get());

	//建立编辑器类型按钮
	this->noteEditorButton = std::make_unique<juce::DrawableButton>(
		"bt_NoteEditor", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->noteEditorButton->setImages(this->iconNoteEditor.get(), nullptr, nullptr, nullptr,
		this->iconNoteEditorHighlight.get(), nullptr, nullptr, nullptr);
	this->noteEditorButton->setLookAndFeel(this->lafs.statusButton);
	this->noteEditorButton->setWantsKeyboardFocus(false);
	this->noteEditorButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->noteEditorButton->setCommandToTrigger(this->commandManager, this->noteEditorCommandID, true);
	this->addAndMakeVisible(this->noteEditorButton.get());

	this->mixEditorButton = std::make_unique<juce::DrawableButton>(
		"bt_MixEditor", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->mixEditorButton->setImages(this->iconMixEditor.get(), nullptr, nullptr, nullptr,
		this->iconMixEditorHighlight.get(), nullptr, nullptr, nullptr);
	this->mixEditorButton->setLookAndFeel(this->lafs.statusButton);
	this->mixEditorButton->setWantsKeyboardFocus(false);
	this->mixEditorButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->mixEditorButton->setCommandToTrigger(this->commandManager, this->mixEditorCommandID, true);
	this->addAndMakeVisible(this->mixEditorButton.get());

	this->scriptEditorButton = std::make_unique<juce::DrawableButton>(
		"bt_ScriptEditor", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->scriptEditorButton->setImages(this->iconScriptEditor.get(), nullptr, nullptr, nullptr,
		this->iconScriptEditorHighlight.get(), nullptr, nullptr, nullptr);
	this->scriptEditorButton->setLookAndFeel(this->lafs.statusButton);
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
	this->noteEditorPluginButton->setLookAndFeel(this->lafs.statusTextButton);
	this->addAndMakeVisible(this->noteEditorPluginButton.get());

	this->noteEditorAdditionPluginButton = std::make_unique<juce::TextButton>(
		this->tr("bt_NoteEditorAdditionPlugin"), this->tr("tip_NoteEditorAdditionPlugin"));
	this->noteEditorAdditionPluginButton->setCommandToTrigger(this->commandManager, this->noteEditorAdditionPluginCommandID, true);
	this->noteEditorAdditionPluginButton->setWantsKeyboardFocus(false);
	this->noteEditorAdditionPluginButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->noteEditorAdditionPluginButton->setLookAndFeel(this->lafs.statusTextButton);
	this->addAndMakeVisible(this->noteEditorAdditionPluginButton.get());
}

void StatusBar::selectNoteEditorPlugin()
{
	juce::PopupMenu menu;

	menu.showAt(this->noteEditorPluginButton.get(), 0, 0, 10);
}

void StatusBar::selectNoteEditionAdditionPlugin()
{
	juce::PopupMenu menu;

	menu.showAt(this->noteEditorAdditionPluginButton.get(), 0, 0, 10);
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
}

void StatusBar::initCommandFunction()
{
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Note Editor Plugin", [this] {
			this->selectNoteEditorPlugin();
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Note Editor Addition Plugin", [this] {
			this->selectNoteEditionAdditionPlugin();
		}
	);
}

void StatusBar::initCommandFlagHook()
{

}
