#include "PlayBar.h"
#include <libJModule.h>

PlayBar::PlayBar()
	:Component()
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
		"main", "color", "background-playBar", this->colors.background_playBar, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-playBarButton", this->colors.icon_playBarButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-playBarButton", this->colors.background_playBarButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-playBarButton-highlight", this->colors.icon_playBarButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-playBarButton-highlight", this->colors.background_playBarButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-playBar", this->colors.text_playBar, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-playBar-value", this->colors.text_playBar_value, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "split-playBar", this->colors.split_playBar, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playMarginLeft", this->sizes.width_playMarginLeft, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playMarginRight", this->sizes.width_playMarginRight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playButtonSplit", this->sizes.width_playButtonSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playButtonGroupSplit", this->sizes.width_playButtonGroupSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playTextButtonMargin", this->sizes.width_playTextButtonMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playTextGroupSplit", this->sizes.width_playTextGroupSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playTextItemSplit", this->sizes.width_playTextItemSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playTextSplit", this->sizes.width_playTextSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-playSplitLine", this->sizes.width_playSplitLine, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-playButton", this->scales.height_playButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-playTextButton", this->scales.height_playTextButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-playSplitLine", this->scales.height_playSplitLine, result
		);

	//resource
	juce::String iconPlayButton, iconStopButton, iconBeginButton, iconEndButton,
		iconLoopButton, iconFollowButton, iconUndoButton, iconRedoButton,
		iconConfigButton;
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-playButton", iconPlayButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-stopButton", iconStopButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-beginButton", iconBeginButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-endButton", iconEndButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-loopButton", iconLoopButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-followButton", iconFollowButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-undo", iconUndoButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-redo", iconRedoButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-config", iconConfigButton, result
		);

	//加载播放按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconPlayButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconPlay = juce::Drawable::createFromSVG(*ptrXml);
				this->iconPlayHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconPlay) {
					this->iconPlay->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
				if (this->iconPlayHighlight) {
					this->iconPlayHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton_highlight
					);
				}
			}
		}
	}

	//加载停止按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconStopButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconStop = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconStop) {
					this->iconStop->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
			}
		}
	}
	
	//加载向前按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconBeginButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconBegin = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconBegin) {
					this->iconBegin->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
			}
		}
	}

	//加载向后按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconEndButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconEnd = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconEnd) {
					this->iconEnd->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
			}
		}
	}

	//加载循环按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconLoopButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconLoop = juce::Drawable::createFromSVG(*ptrXml);
				this->iconLoopHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconLoop) {
					this->iconLoop->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
				if (this->iconLoopHighlight) {
					this->iconLoopHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton_highlight
					);
				}
			}
		}
	}

	//加载跟随按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconFollowButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconFollow = juce::Drawable::createFromSVG(*ptrXml);
				this->iconFollowHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconFollow) {
					this->iconFollow->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
				if (this->iconFollowHighlight) {
					this->iconFollowHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton_highlight
					);
				}
			}
		}
	}

	//加载撤销按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconUndoButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconUndo = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconUndo) {
					this->iconUndo->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
			}
		}
	}

	//加载重做按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconRedoButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconRedo = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconRedo) {
					this->iconRedo->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
			}
		}
	}

	//加载设置按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconConfigButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconConfig = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconConfig) {
					this->iconConfig->replaceColour(
						juce::Colours::black, this->colors.icon_playBarButton
					);
				}
			}
		}
	}

	//建立按钮样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetPlayButtonLAF",
		this->lafs.playButton
		);
	this->lafs.playButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_playBarButton
	);
	this->lafs.playButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_playBarButton_highlight
	);
	this->lafs.playButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//初始化播放按钮
	this->playButton = std::make_unique<juce::DrawableButton>(
		"bt_Play", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->playButton->setImages(this->iconPlay.get(), nullptr, nullptr, nullptr,
		this->iconPlayHighlight.get(), nullptr, nullptr, nullptr);
	this->playButton->setLookAndFeel(this->lafs.playButton);
	this->playButton->setWantsKeyboardFocus(false);
	this->playButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->playButton->setCommandToTrigger(this->commandManager, this->playCommandID, true);
	this->addAndMakeVisible(this->playButton.get());

	//初始化停止按钮
	this->stopButton = std::make_unique<juce::DrawableButton>(
		"bt_Stop", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->stopButton->setImages(this->iconStop.get());
	this->stopButton->setLookAndFeel(this->lafs.playButton);
	this->stopButton->setWantsKeyboardFocus(false);
	this->stopButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->stopButton->setCommandToTrigger(this->commandManager, this->stopCommandID, true);
	this->addAndMakeVisible(this->stopButton.get());

	//初始化向前按钮
	this->beginButton = std::make_unique<juce::DrawableButton>(
		"bt_Begin", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->beginButton->setImages(this->iconBegin.get());
	this->beginButton->setLookAndFeel(this->lafs.playButton);
	this->beginButton->setWantsKeyboardFocus(false);
	this->beginButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->beginButton->setCommandToTrigger(this->commandManager, this->beginCommandID, true);
	this->addAndMakeVisible(this->beginButton.get());

	//初始化向后按钮
	this->endButton = std::make_unique<juce::DrawableButton>(
		"bt_End", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->endButton->setImages(this->iconEnd.get());
	this->endButton->setLookAndFeel(this->lafs.playButton);
	this->endButton->setWantsKeyboardFocus(false);
	this->endButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->endButton->setCommandToTrigger(this->commandManager, this->endCommandID, true);
	this->addAndMakeVisible(this->endButton.get());

	//初始化循环按钮
	this->loopButton = std::make_unique<juce::DrawableButton>(
		"bt_Loop", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->loopButton->setImages(this->iconLoop.get(), nullptr, nullptr, nullptr,
		this->iconLoopHighlight.get(), nullptr, nullptr, nullptr);
	this->loopButton->setLookAndFeel(this->lafs.playButton);
	this->loopButton->setWantsKeyboardFocus(false);
	this->loopButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->loopButton->setCommandToTrigger(this->commandManager, this->loopCommandID, true);
	this->addAndMakeVisible(this->loopButton.get());

	//初始化跟随按钮
	this->followButton = std::make_unique<juce::DrawableButton>(
		"bt_Follow", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->followButton->setImages(this->iconFollow.get(), nullptr, nullptr, nullptr,
		this->iconFollowHighlight.get(), nullptr, nullptr, nullptr);
	this->followButton->setLookAndFeel(this->lafs.playButton);
	this->followButton->setWantsKeyboardFocus(false);
	this->followButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->followButton->setCommandToTrigger(this->commandManager, this->followCommandID, true);
	this->addAndMakeVisible(this->followButton.get());

	//初始化撤销按钮
	this->undoButton = std::make_unique<juce::DrawableButton>(
		"bt_Undo", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->undoButton->setImages(this->iconUndo.get());
	this->undoButton->setLookAndFeel(this->lafs.playButton);
	this->undoButton->setWantsKeyboardFocus(false);
	this->undoButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->undoButton->setCommandToTrigger(this->commandManager, this->undoCommandID, true);
	this->addAndMakeVisible(this->undoButton.get());

	//初始化重做按钮
	this->redoButton = std::make_unique<juce::DrawableButton>(
		"bt_Redo", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->redoButton->setImages(this->iconRedo.get());
	this->redoButton->setLookAndFeel(this->lafs.playButton);
	this->redoButton->setWantsKeyboardFocus(false);
	this->redoButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->redoButton->setCommandToTrigger(this->commandManager, this->redoCommandID, true);
	this->addAndMakeVisible(this->redoButton.get());

	//初始化设置按钮
	this->configButton = std::make_unique<juce::DrawableButton>(
		"bt_Config", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->configButton->setImages(this->iconConfig.get());
	this->configButton->setLookAndFeel(this->lafs.playButton);
	this->configButton->setWantsKeyboardFocus(false);
	this->configButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	//this->configButton->setCommandToTrigger(this->commandManager, this->configCommandID, true);
	this->addAndMakeVisible(this->configButton.get());

	//预生成字符串
	this->textAdsorb = this->tr("lb_Adsorb");
	this->textGrid = this->tr("lb_Grid");

	//建立选择按钮样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetPlayTextButtonLAF",
		this->lafs.playTextButton
		);
	this->lafs.playTextButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_playBar
	);
	this->lafs.playTextButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_playBar
	);
	this->lafs.playTextButton->setColour(
		juce::TextButton::ColourIds::textColourOnId, this->colors.text_playBar_value
	);
	this->lafs.playTextButton->setColour(
		juce::TextButton::ColourIds::textColourOffId, this->colors.text_playBar_value
	);
	this->lafs.playTextButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//初始化吸附按钮
	this->adsorbButton = std::make_unique<juce::TextButton>(
		juce::String(), this->textAdsorb);
	this->adsorbButton->setWantsKeyboardFocus(false);
	this->adsorbButton->setFocusContainerType(juce::Component::FocusContainerType::none);
	this->adsorbButton->setLookAndFeel(this->lafs.playTextButton);
	this->adsorbButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->adsorbButton->onClick = [this] {this->showAdsorbMenu(); };
	this->addAndMakeVisible(this->adsorbButton.get());

	//初始化网格按钮
	this->gridButton = std::make_unique<juce::TextButton>(
		juce::String(), this->textGrid);
	this->gridButton->setWantsKeyboardFocus(false);
	this->gridButton->setFocusContainerType(juce::Component::FocusContainerType::none);
	this->gridButton->setLookAndFeel(this->lafs.playTextButton);
	this->gridButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->gridButton->onClick = [this] {this->showGridMenu(); };
	this->addAndMakeVisible(this->gridButton.get());

	//更新按钮
	this->refreshAdsorbButton();
	this->refreshGridButton();
}

void PlayBar::play()
{
	//TODO
}

void PlayBar::stop()
{
	//TODO
}

void PlayBar::begin()
{
	//TODO
}

void PlayBar::end()
{
	//TODO
}

void PlayBar::loop()
{
	//TODO
}

void PlayBar::follow()
{
	//TODO
}

void PlayBar::backOnStop()
{
	//TODO
}

bool PlayBar::isPlaying()
{
	//TODO
	return false;
}

bool PlayBar::isLoop()
{
	//TODO
	return false;
}

bool PlayBar::isFollow()
{
	//TODO
	return true;
}

bool PlayBar::isBackOnStop()
{
	//TODO
	return false;
}

void PlayBar::resized()
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int width_leftMargin = this->sizes.width_playMarginLeft * screenSize.getWidth();
	int width_buttonSplit = this->sizes.width_playButtonSplit * screenSize.getWidth();
	int width_buttonGroupSplit = this->sizes.width_playButtonGroupSplit * screenSize.getWidth();
	int width_splitLine = this->sizes.width_playSplitLine * screenSize.getWidth();
	int height_button = this->scales.height_playButton * this->getHeight();
	int width_button = height_button;
	int height_splitLine = this->scales.height_playSplitLine * this->getHeight();

	int width_rightMargin = this->sizes.width_playMarginRight * screenSize.getWidth();

	int width_textButtonMargin = this->sizes.width_playTextButtonMargin * screenSize.getWidth();
	int width_textSplit = this->sizes.width_playTextSplit * screenSize.getWidth();
	int width_textItemSplit = this->sizes.width_playTextItemSplit * screenSize.getWidth();
	int width_textGroupSplit = this->sizes.width_playTextGroupSplit * screenSize.getWidth();
	int height_textButton = this->scales.height_playTextButton * this->getHeight();

	//通过label作为字体标准
	juce::Font font =
		juce::LookAndFeel::getDefaultLookAndFeel().getTextButtonFont(*(this->adsorbButton.get()), height_textButton);

	//计算文字宽度
	int width_textAdsorb = font.getStringWidth(this->textAdsorb);
	int width_textGrid = font.getStringWidth(this->textGrid);
	int width_textAdsorbButton = font.getStringWidth(this->adsorbButton->getButtonText());
	int width_textGridButton = font.getStringWidth(this->gridButton->getButtonText());

	//计算文字按钮宽度
	int width_adsorbButton = width_textAdsorbButton + width_textButtonMargin * 2;
	int width_gridButton = width_textGridButton + width_textButtonMargin * 2;

	//调整左侧按钮位置
	this->playButton->setBounds(
		width_leftMargin + width_splitLine * 0 + width_button * 0 + width_buttonGroupSplit * 0,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);
	this->stopButton->setBounds(
		width_leftMargin + width_splitLine * 1 + width_button * 1 + width_buttonGroupSplit * 0,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);
	this->beginButton->setBounds(
		width_leftMargin + width_splitLine * 2 + width_button * 2 + width_buttonGroupSplit * 0,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);
	this->endButton->setBounds(
		width_leftMargin + width_splitLine * 3 + width_button * 3 + width_buttonGroupSplit * 0,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);
	this->loopButton->setBounds(
		width_leftMargin + width_splitLine * 4 + width_button * 4 + width_buttonGroupSplit * 1,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);
	this->followButton->setBounds(
		width_leftMargin + width_splitLine * 5 + width_button * 5 + width_buttonGroupSplit * 1,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);

	//调整中间按钮位置
	this->undoButton->setBounds(
		this->getWidth() / 2 - width_buttonSplit / 2 - width_button,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);
	this->redoButton->setBounds(
		this->getWidth() / 2 + width_buttonSplit / 2,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);

	//调整右侧按钮位置
	this->configButton->setBounds(
		this->getWidth() - width_rightMargin - width_button,
		this->getHeight() / 2 - height_button / 2,
		width_button, height_button
	);

	//调整右侧选项位置
	this->gridButton->setBounds(
		this->getWidth() - width_rightMargin - width_button - width_buttonGroupSplit
		- width_gridButton,
		this->getHeight() / 2 - height_textButton / 2,
		width_gridButton, height_textButton
	);
	this->adsorbButton->setBounds(
		this->getWidth() - width_rightMargin - width_button - width_buttonGroupSplit
		- width_gridButton - width_textSplit - width_textGrid - width_textItemSplit
		- width_adsorbButton,
		this->getHeight() / 2 - height_textButton / 2,
		width_adsorbButton, height_textButton
	);
}

void PlayBar::paint(juce::Graphics& g)
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//绘制背景
	g.fillAll(this->colors.background_playBar);

	//计算控件大小
	int width_leftMargin = this->sizes.width_playMarginLeft * screenSize.getWidth();
	int width_buttonSplit = this->sizes.width_playButtonSplit * screenSize.getWidth();
	int width_buttonGroupSplit = this->sizes.width_playButtonGroupSplit * screenSize.getWidth();
	int width_splitLine = this->sizes.width_playSplitLine * screenSize.getWidth();
	int height_button = this->scales.height_playButton * this->getHeight();
	int width_button = height_button;
	int height_splitLine = this->scales.height_playSplitLine * this->getHeight();

	int width_rightMargin = this->sizes.width_playMarginRight * screenSize.getWidth();

	int width_textButtonMargin = this->sizes.width_playTextButtonMargin * screenSize.getWidth();
	int width_textSplit = this->sizes.width_playTextSplit * screenSize.getWidth();
	int width_textItemSplit = this->sizes.width_playTextItemSplit * screenSize.getWidth();
	int width_textGroupSplit = this->sizes.width_playTextGroupSplit * screenSize.getWidth();
	int height_textButton = this->scales.height_playTextButton * this->getHeight();

	//通过label作为字体标准
	juce::Font font =
		juce::LookAndFeel::getDefaultLookAndFeel().getTextButtonFont(*(this->adsorbButton.get()), height_textButton);
	g.setFont(font);

	//计算文字宽度
	int width_textAdsorb = font.getStringWidth(this->textAdsorb);
	int width_textGrid = font.getStringWidth(this->textGrid);
	int width_textAdsorbButton = font.getStringWidth(this->adsorbButton->getButtonText());
	int width_textGridButton = font.getStringWidth(this->gridButton->getButtonText());

	//计算文字按钮宽度
	int width_adsorbButton = width_textAdsorbButton + width_textButtonMargin * 2;
	int width_gridButton = width_textGridButton + width_textButtonMargin * 2;
	
	//计算分割线位置
	int posXLine1 = width_leftMargin + width_splitLine * 4 + width_button * 4
		+ width_buttonGroupSplit / 2;
	int posXLine2 = this->getWidth() - width_rightMargin - width_button
		- width_buttonGroupSplit / 2;

	//绘分割线
	g.setColour(this->colors.split_playBar);
	g.drawLine(
		posXLine1, this->getHeight() / 2.f - height_splitLine / 2.f,
		posXLine1, this->getHeight() / 2.f + height_splitLine / 2.f,
		width_splitLine
	);
	g.drawLine(
		posXLine2, this->getHeight() / 2.f - height_splitLine / 2.f,
		posXLine2, this->getHeight() / 2.f + height_splitLine / 2.f,
		width_splitLine
	);

	//计算右侧文字位置
	juce::Rectangle<int> rectTextGrid(
		this->getWidth() - width_rightMargin - width_button - width_buttonGroupSplit
		- width_gridButton - width_textSplit - width_textGrid,
		this->getHeight() / 2 - height_textButton / 2,
		width_textGrid, height_textButton
	);
	juce::Rectangle<int> rectTextAdsorb(
		this->getWidth() - width_rightMargin - width_button - width_buttonGroupSplit
		- width_gridButton - width_textSplit - width_textGrid - width_textItemSplit
		- width_adsorbButton - width_textSplit - width_textAdsorb,
		this->getHeight() / 2 - height_textButton / 2,
		width_textAdsorb, height_textButton
	);

	//绘制右侧文字
	g.setColour(this->colors.text_playBar);
	g.drawFittedText(this->textAdsorb, rectTextAdsorb,
		juce::Justification::centred, 1);
	g.drawFittedText(this->textGrid, rectTextGrid,
		juce::Justification::centred, 1);
}

PlayBar::AdsorbState PlayBar::getAdsorb()
{
	return this->adsorbState;
}

void PlayBar::setAdsorb(PlayBar::AdsorbState state)
{
	this->adsorbState = state;
	this->refreshAdsorbButton();
}

PlayBar::GridState PlayBar::getGrid()
{
	return this->gridState;
}

void PlayBar::setGrid(PlayBar::GridState state)
{
	this->gridState = state;
	this->refreshGridButton();
}

void PlayBar::showAdsorbMenu()
{
	this->createAdsorbMenu().showAt(this->adsorbButton.get());
}

void PlayBar::showGridMenu()
{
	this->createGridMenu().showAt(this->gridButton.get());
}

void PlayBar::initCommandID()
{
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Play", this->playCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Stop", this->stopCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Begin", this->beginCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"End", this->endCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Loop", this->loopCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Follow", this->followCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Back On Stop", this->backOnStopCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Undo", this->undoCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Redo", this->redoCommandID
		);
	//Adsorb
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1 Beat", this->commandAdsorb1Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1/2 Beat", this->commandAdsorb1_2Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1/4 Beat", this->commandAdsorb1_4Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1/6 Beat", this->commandAdsorb1_6Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1/8 Beat", this->commandAdsorb1_8Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1/12 Beat", this->commandAdsorb1_12Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1/16 Beat", this->commandAdsorb1_16Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1/24 Beat", this->commandAdsorb1_24Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb 1/32 Beat", this->commandAdsorb1_32Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Adsorb Off", this->commandAdsorbOff
		);

	//Grid
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1 Beat", this->commandGrid1Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1/2 Beat", this->commandGrid1_2Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1/4 Beat", this->commandGrid1_4Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1/6 Beat", this->commandGrid1_6Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1/8 Beat", this->commandGrid1_8Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1/12 Beat", this->commandGrid1_12Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1/16 Beat", this->commandGrid1_16Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1/24 Beat", this->commandGrid1_24Beat
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Grid 1/32 Beat", this->commandGrid1_32Beat
		);
}

void PlayBar::initCommandFunction()
{
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Play", [this] {this->play(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Stop", [this] {this->stop(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Begin", [this] {this->begin(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"End", [this] {this->end(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Loop", [this] {this->loop(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Follow", [this] {this->follow(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Back On Stop", [this] {this->backOnStop(); }
	);
	//Adsorb
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1/2 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1_2Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1/4 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1_4Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1/6 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1_6Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1/8 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1_8Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1/12 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1_12Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1/16 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1_16Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1/24 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1_24Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb 1/32 Beat", [this] {this->setAdsorb(AdsorbState::Adsorb1_32Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Adsorb Off", [this] {this->setAdsorb(AdsorbState::AdsorbOff); }
	);
	//Grid
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1 Beat", [this] {this->setGrid(GridState::Grid1Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1/2 Beat", [this] {this->setGrid(GridState::Grid1_2Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1/4 Beat", [this] {this->setGrid(GridState::Grid1_4Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1/6 Beat", [this] {this->setGrid(GridState::Grid1_6Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1/8 Beat", [this] {this->setGrid(GridState::Grid1_8Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1/12 Beat", [this] {this->setGrid(GridState::Grid1_12Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1/16 Beat", [this] {this->setGrid(GridState::Grid1_16Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1/24 Beat", [this] {this->setGrid(GridState::Grid1_24Beat); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Grid 1/32 Beat", [this] {this->setGrid(GridState::Grid1_32Beat); }
	);
}

void PlayBar::initCommandFlagHook()
{
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Play", [this]()->int {
			int flag = 0;
			if (this->isPlaying()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Stop", [this]()->int {
			int flag = 0;
			if (!this->isPlaying()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Loop", [this]()->int {
			int flag = 0;
			if (this->isLoop()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Follow", [this]()->int {
			int flag = 0;
			if (this->isFollow()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Back On Stop", [this]()->int {
			int flag = 0;
			if (this->isBackOnStop()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	//Adsorb
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/2 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1_2Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/4 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1_4Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/6 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1_6Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/8 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1_8Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/12 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1_12Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/16 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1_16Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/24 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1_24Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/32 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::Adsorb1_32Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb Off", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == AdsorbState::AdsorbOff) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	//Grid
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/2 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1_2Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/4 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1_4Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/6 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1_6Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/8 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1_8Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/12 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1_12Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/16 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1_16Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/24 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1_24Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/32 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == GridState::Grid1_32Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
}

void PlayBar::refreshAdsorbButton()
{
	juce::String buttonText;
	switch (this->adsorbState)
	{
	case AdsorbState::Adsorb1Beat:
		buttonText = this->tr("sn_Adsorb1Beat");
		break;
	case AdsorbState::Adsorb1_2Beat:
		buttonText = this->tr("sn_Adsorb1/2Beat");
		break;
	case AdsorbState::Adsorb1_4Beat:
		buttonText = this->tr("sn_Adsorb1/4Beat");
		break;
	case AdsorbState::Adsorb1_6Beat:
		buttonText = this->tr("sn_Adsorb1/6Beat");
		break;
	case AdsorbState::Adsorb1_8Beat:
		buttonText = this->tr("sn_Adsorb1/8Beat");
		break;
	case AdsorbState::Adsorb1_12Beat:
		buttonText = this->tr("sn_Adsorb1/12Beat");
		break;
	case AdsorbState::Adsorb1_16Beat:
		buttonText = this->tr("sn_Adsorb1/16Beat");
		break;
	case AdsorbState::Adsorb1_24Beat:
		buttonText = this->tr("sn_Adsorb1/24Beat");
		break;
	case AdsorbState::Adsorb1_32Beat:
		buttonText = this->tr("sn_Adsorb1/32Beat");
		break;
	case AdsorbState::AdsorbOff:
		buttonText = this->tr("sn_AdsorbOff");
		break;
	}

	this->adsorbButton->setButtonText(buttonText);
	this->resized();
	this->repaint();
}

void PlayBar::refreshGridButton()
{
	juce::String buttonText;
	switch (this->gridState)
	{
	case GridState::Grid1Beat:
		buttonText = this->tr("sn_Grid1Beat");
		break;
	case GridState::Grid1_2Beat:
		buttonText = this->tr("sn_Grid1/2Beat");
		break;
	case GridState::Grid1_4Beat:
		buttonText = this->tr("sn_Grid1/4Beat");
		break;
	case GridState::Grid1_6Beat:
		buttonText = this->tr("sn_Grid1/6Beat");
		break;
	case GridState::Grid1_8Beat:
		buttonText = this->tr("sn_Grid1/8Beat");
		break;
	case GridState::Grid1_12Beat:
		buttonText = this->tr("sn_Grid1/12Beat");
		break;
	case GridState::Grid1_16Beat:
		buttonText = this->tr("sn_Grid1/16Beat");
		break;
	case GridState::Grid1_24Beat:
		buttonText = this->tr("sn_Grid1/24Beat");
		break;
	case GridState::Grid1_32Beat:
		buttonText = this->tr("sn_Grid1/32Beat");
		break;
	}

	this->gridButton->setButtonText(buttonText);
	this->resized();
	this->repaint();
}

juce::PopupMenu PlayBar::createAdsorbMenu()
{
	juce::PopupMenu menu;

	menu.addCommandItem(this->commandManager, this->commandAdsorb1Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorb1_2Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorb1_4Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorb1_6Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorb1_8Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorb1_12Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorb1_16Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorb1_24Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorb1_32Beat);
	menu.addCommandItem(this->commandManager, this->commandAdsorbOff);

	return menu;
}

juce::PopupMenu PlayBar::createGridMenu()
{
	juce::PopupMenu menu;

	menu.addCommandItem(this->commandManager, this->commandGrid1Beat);
	menu.addCommandItem(this->commandManager, this->commandGrid1_2Beat);
	menu.addCommandItem(this->commandManager, this->commandGrid1_4Beat);
	menu.addCommandItem(this->commandManager, this->commandGrid1_6Beat);
	menu.addCommandItem(this->commandManager, this->commandGrid1_8Beat);
	menu.addCommandItem(this->commandManager, this->commandGrid1_12Beat);
	menu.addCommandItem(this->commandManager, this->commandGrid1_16Beat);
	menu.addCommandItem(this->commandManager, this->commandGrid1_24Beat);
	menu.addCommandItem(this->commandManager, this->commandGrid1_32Beat);

	return menu;
}
