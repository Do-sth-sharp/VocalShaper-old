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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-viewModeButton", iconViewModeButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-editModeButton", iconEditModeButton, result
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

	//建立按钮样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetToolButtonLAF",
		this->lafs.toolButton
		);
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
	this->viewModeButton->setLookAndFeel(this->lafs.toolButton);
	this->viewModeButton->setWantsKeyboardFocus(false);
	this->viewModeButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->viewModeButton->setCommandToTrigger(this->commandManager, this->viewModeCommandID, true);
	this->addAndMakeVisible(this->viewModeButton.get());

	//初始化编辑模式按钮
	this->editModeButton = std::make_unique<juce::DrawableButton>(
		"bt_EditMode", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->editModeButton->setImages(this->iconEditMode.get(), nullptr, nullptr, nullptr,
		this->iconEditModeHighlight.get(), nullptr, nullptr, nullptr);
	this->editModeButton->setLookAndFeel(this->lafs.toolButton);
	this->editModeButton->setWantsKeyboardFocus(false);
	this->editModeButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->editModeButton->setCommandToTrigger(this->commandManager, this->editModeCommandID, true);
	this->addAndMakeVisible(this->editModeButton.get());
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
}

void ToolBar::paint(juce::Graphics& g)
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//填充背景
	g.fillAll(this->colors.background_toolBar);

	//计算上边框大小并绘制
	int height_topBorder = this->sizes.height_toolBorderTop * screenSize.getHeight();
	juce::Rectangle<int> rectTopBorder(
		0, 0,
		this->getWidth(), height_topBorder
	);
	g.setColour(this->colors.border_toolBar);
	g.fillRect(rectTopBorder);
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
}

void ToolBar::initCommandFunction()
{

}

void ToolBar::initCommandFlagHook()
{

}
