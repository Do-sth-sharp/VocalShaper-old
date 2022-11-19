#include "ToolBar.h"
#include <libJModule.h>

ToolBar::ToolBar()
	: Component("ToolBar")
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
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
		"main", "color", "background-toolBar", this->colors.background_toolBar, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-toolBarButton", this->colors.icon_toolBarButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-toolBarButton", this->colors.background_toolBarButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "icon-toolBarButton-highlight", this->colors.icon_toolBarButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-toolBarButton-highlight", this->colors.background_toolBarButton_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "split-toolBar", this->colors.split_toolBar, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border-toolBar", this->colors.border_toolBar, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-toolMarginTop", this->sizes.height_toolMarginTop, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-toolButtonSplit", this->sizes.height_toolButtonSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-toolButtonGroupSplit", this->sizes.height_toolButtonGroupSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-toolSplitLine", this->sizes.height_toolSplitLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-toolBorderTop", this->sizes.height_toolBorderTop, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "width-toolButton", this->scales.width_toolButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "width-toolSplitLine", this->scales.width_toolSplitLine, result
		);

	//resource
	juce::String iconViewModeButton, iconEditModeButton;
	juce::String iconTool1Button, iconTool2Button, iconTool3Button, iconTool4Button,
		iconTool5Button;
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-viewModeButton", iconViewModeButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-editModeButton", iconEditModeButton, result
		);

	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-tool1Button", iconTool1Button, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-tool2Button", iconTool2Button, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-tool3Button", iconTool3Button, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-tool4Button", iconTool4Button, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-tool5Button", iconTool5Button, result
		);
	
	//加载浏览模式按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconViewModeButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconViewMode = juce::Drawable::createFromSVG(*ptrXml);
				this->iconViewModeHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconViewMode) {
					this->iconViewMode->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton
					);
				}
				if (this->iconViewModeHighlight) {
					this->iconViewModeHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton_highlight
					);
				}
			}
		}
	}

	//加载编辑模式按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconEditModeButton;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconEditMode = juce::Drawable::createFromSVG(*ptrXml);
				this->iconEditModeHighlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconEditMode) {
					this->iconEditMode->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton
					);
				}
				if (this->iconEditModeHighlight) {
					this->iconEditModeHighlight->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton_highlight
					);
				}
			}
		}
	}

	//加载工具1按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconTool1Button;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconTool1 = juce::Drawable::createFromSVG(*ptrXml);
				this->iconTool1Highlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconTool1) {
					this->iconTool1->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton
					);
				}
				if (this->iconTool1Highlight) {
					this->iconTool1Highlight->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton_highlight
					);
				}
			}
		}
	}

	//加载工具2按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconTool2Button;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconTool2 = juce::Drawable::createFromSVG(*ptrXml);
				this->iconTool2Highlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconTool2) {
					this->iconTool2->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton
					);
				}
				if (this->iconTool2Highlight) {
					this->iconTool2Highlight->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton_highlight
					);
				}
			}
		}
	}

	//加载工具3按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconTool3Button;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconTool3 = juce::Drawable::createFromSVG(*ptrXml);
				this->iconTool3Highlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconTool3) {
					this->iconTool3->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton
					);
				}
				if (this->iconTool3Highlight) {
					this->iconTool3Highlight->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton_highlight
					);
				}
			}
		}
	}

	//加载工具4按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconTool4Button;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconTool4 = juce::Drawable::createFromSVG(*ptrXml);
				this->iconTool4Highlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconTool4) {
					this->iconTool4->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton
					);
				}
				if (this->iconTool4Highlight) {
					this->iconTool4Highlight->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton_highlight
					);
				}
			}
		}
	}

	//加载工具5按钮图标
	{
		size_t iconSize = 0;
		void* iconPtr = nullptr;
		juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + iconTool5Button;

		jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC",
			"GetRC",
			iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
			);
		if (iconPtr) {
			juce::String iconStr((char*)iconPtr, iconSize);
			auto ptrXml = juce::XmlDocument::parse(iconStr);
			if (ptrXml) {
				this->iconTool5 = juce::Drawable::createFromSVG(*ptrXml);
				this->iconTool5Highlight = juce::Drawable::createFromSVG(*ptrXml);
				if (this->iconTool5) {
					this->iconTool5->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton
					);
				}
				if (this->iconTool5Highlight) {
					this->iconTool5Highlight->replaceColour(
						juce::Colours::black, this->colors.icon_toolBarButton_highlight
					);
				}
			}
		}
	}

	//建立按钮样式
	this->lafs.toolButton = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.toolButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_toolBarButton
	);
	this->lafs.toolButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_toolBarButton_highlight
	);
	this->lafs.toolButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//初始化浏览模式按钮
	this->viewModeButton = std::make_unique<juce::DrawableButton>(
		"bt_ViewMode", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->viewModeButton->setImages(this->iconViewMode.get(), nullptr, nullptr, nullptr,
		this->iconViewModeHighlight.get(), nullptr, nullptr, nullptr);
	this->viewModeButton->setLookAndFeel(this->lafs.toolButton.get());
	this->viewModeButton->setWantsKeyboardFocus(false);
	this->viewModeButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->viewModeButton->setCommandToTrigger(this->commandManager, this->viewModeCommandID, true);
	this->addAndMakeVisible(this->viewModeButton.get());

	//初始化编辑模式按钮
	this->editModeButton = std::make_unique<juce::DrawableButton>(
		"bt_EditMode", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->editModeButton->setImages(this->iconEditMode.get(), nullptr, nullptr, nullptr,
		this->iconEditModeHighlight.get(), nullptr, nullptr, nullptr);
	this->editModeButton->setLookAndFeel(this->lafs.toolButton.get());
	this->editModeButton->setWantsKeyboardFocus(false);
	this->editModeButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->editModeButton->setCommandToTrigger(this->commandManager, this->editModeCommandID, true);
	this->addAndMakeVisible(this->editModeButton.get());

	//初始化工具1按钮
	this->tool1Button = std::make_unique<juce::DrawableButton>(
		"bt_Tool1", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->tool1Button->setImages(this->iconTool1.get(), nullptr, nullptr, nullptr,
		this->iconTool1Highlight.get(), nullptr, nullptr, nullptr);
	this->tool1Button->setLookAndFeel(this->lafs.toolButton.get());
	this->tool1Button->setWantsKeyboardFocus(false);
	this->tool1Button->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->tool1Button->setCommandToTrigger(this->commandManager, this->tool1CommandID, true);
	this->addAndMakeVisible(this->tool1Button.get());

	//初始化工具2按钮
	this->tool2Button = std::make_unique<juce::DrawableButton>(
		"bt_Tool2", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->tool2Button->setImages(this->iconTool2.get(), nullptr, nullptr, nullptr,
		this->iconTool2Highlight.get(), nullptr, nullptr, nullptr);
	this->tool2Button->setLookAndFeel(this->lafs.toolButton.get());
	this->tool2Button->setWantsKeyboardFocus(false);
	this->tool2Button->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->tool2Button->setCommandToTrigger(this->commandManager, this->tool2CommandID, true);
	this->addAndMakeVisible(this->tool2Button.get());

	//初始化工具3按钮
	this->tool3Button = std::make_unique<juce::DrawableButton>(
		"bt_Tool3", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->tool3Button->setImages(this->iconTool3.get(), nullptr, nullptr, nullptr,
		this->iconTool3Highlight.get(), nullptr, nullptr, nullptr);
	this->tool3Button->setLookAndFeel(this->lafs.toolButton.get());
	this->tool3Button->setWantsKeyboardFocus(false);
	this->tool3Button->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->tool3Button->setCommandToTrigger(this->commandManager, this->tool3CommandID, true);
	this->addAndMakeVisible(this->tool3Button.get());

	//初始化工具4按钮
	this->tool4Button = std::make_unique<juce::DrawableButton>(
		"bt_Tool4", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->tool4Button->setImages(this->iconTool4.get(), nullptr, nullptr, nullptr,
		this->iconTool4Highlight.get(), nullptr, nullptr, nullptr);
	this->tool4Button->setLookAndFeel(this->lafs.toolButton.get());
	this->tool4Button->setWantsKeyboardFocus(false);
	this->tool4Button->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->tool4Button->setCommandToTrigger(this->commandManager, this->tool4CommandID, true);
	this->addAndMakeVisible(this->tool4Button.get());

	//初始化工具5按钮
	this->tool5Button = std::make_unique<juce::DrawableButton>(
		"bt_Tool5", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->tool5Button->setImages(this->iconTool5.get(), nullptr, nullptr, nullptr,
		this->iconTool5Highlight.get(), nullptr, nullptr, nullptr);
	this->tool5Button->setLookAndFeel(this->lafs.toolButton.get());
	this->tool5Button->setWantsKeyboardFocus(false);
	this->tool5Button->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->tool5Button->setCommandToTrigger(this->commandManager, this->tool5CommandID, true);
	this->addAndMakeVisible(this->tool5Button.get());
}

void ToolBar::resized()
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_topBorder = this->sizes.height_toolBorderTop * screenSize.getHeight();

	int height_topMargin = this->sizes.height_toolMarginTop * screenSize.getHeight();
	int height_buttonSplit = this->sizes.height_toolButtonSplit * screenSize.getHeight();
	int height_buttonGroupSplit = this->sizes.height_toolButtonGroupSplit * screenSize.getHeight();
	int height_splitLine = this->sizes.height_toolSplitLine * screenSize.getHeight();
	int width_button = this->scales.width_toolButton * this->getWidth();
	int height_button = width_button;
	int width_splitLine = this->scales.width_toolSplitLine * this->getWidth();

	//调整按钮位置
	this->viewModeButton->setBounds(
		this->getWidth() / 2 - width_button / 2,
		height_topBorder + height_topMargin + height_buttonSplit * 0 + height_button * 0 + height_buttonGroupSplit * 0,
		width_button, height_button
	);
	this->editModeButton->setBounds(
		this->getWidth() / 2 - width_button / 2,
		height_topBorder + height_topMargin + height_buttonSplit * 1 + height_button * 1 + height_buttonGroupSplit * 0,
		width_button, height_button
	);

	this->tool1Button->setBounds(
		this->getWidth() / 2 - width_button / 2,
		height_topBorder + height_topMargin + height_buttonSplit * 1 + height_button * 2 + height_buttonGroupSplit * 1,
		width_button, height_button
	);
	this->tool2Button->setBounds(
		this->getWidth() / 2 - width_button / 2,
		height_topBorder + height_topMargin + height_buttonSplit * 2 + height_button * 3 + height_buttonGroupSplit * 1,
		width_button, height_button
	);
	this->tool3Button->setBounds(
		this->getWidth() / 2 - width_button / 2,
		height_topBorder + height_topMargin + height_buttonSplit * 3 + height_button * 4 + height_buttonGroupSplit * 1,
		width_button, height_button
	);
	this->tool4Button->setBounds(
		this->getWidth() / 2 - width_button / 2,
		height_topBorder + height_topMargin + height_buttonSplit * 4 + height_button * 5 + height_buttonGroupSplit * 1,
		width_button, height_button
	);
	this->tool5Button->setBounds(
		this->getWidth() / 2 - width_button / 2,
		height_topBorder + height_topMargin + height_buttonSplit * 5 + height_button * 6 + height_buttonGroupSplit * 1,
		width_button, height_button
	);
}

void ToolBar::paint(juce::Graphics& g)
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//填充背景
	g.fillAll(this->colors.background_toolBar);

	//计算控件大小
	int height_topBorder = this->sizes.height_toolBorderTop * screenSize.getHeight();

	int height_topMargin = this->sizes.height_toolMarginTop * screenSize.getHeight();
	int height_buttonSplit = this->sizes.height_toolButtonSplit * screenSize.getHeight();
	int height_buttonGroupSplit = this->sizes.height_toolButtonGroupSplit * screenSize.getHeight();
	int height_splitLine = this->sizes.height_toolSplitLine * screenSize.getHeight();
	int width_button = this->scales.width_toolButton * this->getWidth();
	int height_button = width_button;
	int width_splitLine = this->scales.width_toolSplitLine * this->getWidth();

	//绘制上边框
	juce::Rectangle<int> rectTopBorder(
		0, 0,
		this->getWidth(), height_topBorder
	);
	g.setColour(this->colors.border_toolBar);
	g.fillRect(rectTopBorder);

	//计算分割线位置
	int posYLine1 = height_topBorder + height_topMargin + height_buttonSplit * 1 + height_button * 2
		+ height_buttonGroupSplit / 2;

	//绘制分割线
	g.setColour(this->colors.split_toolBar);
	g.drawLine(
		this->getWidth() / 2.f - width_splitLine / 2.f, posYLine1,
		this->getWidth() / 2.f + width_splitLine / 2.f, posYLine1,
		height_splitLine
	);
}

void ToolBar::initCommandID()
{
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"View Mode", this->viewModeCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Edit Mode", this->editModeCommandID
		);

	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Tool 1", this->tool1CommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Tool 2", this->tool2CommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Tool 3", this->tool3CommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Tool 4", this->tool4CommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Tool 5", this->tool5CommandID
		);
}

void ToolBar::initCommandFunction()
{

}

void ToolBar::initCommandFlagHook()
{

}
