﻿#include "SMComponent.h"
#include <libJModule.h>

SMComponent::SMComponent()
	: Component("Start Menu Component")
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

    //以下获取界面属性
    bool result = false;
    //color
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background", this->colors.background, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "leftBar", this->colors.leftBar, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-button", this->colors.text_button, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-button", this->colors.background_button, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-title", this->colors.text_title, result
        );

    //size
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-font", this->sizes.height_font, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-leftBar", this->sizes.width_leftBar, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-leftBar-max", this->sizes.width_leftBar_max, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-logo-topMargin", this->sizes.height_logo_topMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-button", this->sizes.width_button, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-button", this->sizes.height_button, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-button-logo-split", this->sizes.height_button_logo_split, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-button-split", this->sizes.height_button_split, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-titleBar", this->sizes.height_titleBar, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-titleBar-max", this->sizes.height_titleBar_max, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-title-leftMargin", this->sizes.width_title_leftMargin, result
        );
    //position
    //scale
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "width-logo", this->scales.width_logo, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-title", this->scales.height_title, result
        );

    //resource
    juce::String logoFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "logo", logoFile, result
        );
	
    //以下获取头图
    size_t logoSize = 0;
    void* logoPtr = nullptr;
    juce::String logoPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
        .getParentDirectory().getFullPathName() + logoFile;

    jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
        "WuChang.JMADF.DynamicRC",
        "GetRC",
        logoPath, std::pair<size_t&, void*&>(logoSize, logoPtr)
        );

    this->logoImage = juce::ImageFileFormat::loadFrom(logoPtr, logoSize);

    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);

    //以下构建按钮LookAndFeel
    jmadf::CallInterface<int, juce::LookAndFeel*&>(
        "VocalSharp.VocalShaper.LookAndFeelFactory", "GetStartMenuButtonLAF",
        screenSize.getHeight() * this->sizes.height_font, this->lafs.button
        );
    this->lafs.button->setColour(
        juce::TextButton::ColourIds::buttonColourId, this->colors.background_button);
    this->lafs.button->setColour(
        juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_button);
    this->lafs.button->setColour(
        juce::TextButton::ColourIds::textColourOffId, this->colors.text_button);
    this->lafs.button->setColour(
        juce::TextButton::ColourIds::textColourOnId, this->colors.text_button);
    this->lafs.button->setColour(
        juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
    );

    //以下初始化按钮
    this->btNewProj = std::make_unique<juce::TextButton>(
        this->tr("bt_NewProject"), this->tr("tip_NewProject"));
    this->btOpenProj = std::make_unique<juce::TextButton>(
        this->tr("bt_OpenProject"), this->tr("tip_OpenProject"));

    this->btNewProj->setWantsKeyboardFocus(false);
    this->btOpenProj->setWantsKeyboardFocus(false);

    this->btNewProj->setLookAndFeel(this->lafs.button);
    this->btOpenProj->setLookAndFeel(this->lafs.button);

    this->addAndMakeVisible(this->btNewProj.get());
    this->addAndMakeVisible(this->btOpenProj.get());
}

SMComponent::~SMComponent()
{
}

void SMComponent::resized()
{
    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);

    //计算按钮大小
    int leftBarWidth =
        (int)std::min(this->sizes.width_leftBar * screenSize.getWidth(), this->sizes.width_leftBar_max);
    double logoScale = (leftBarWidth * this->scales.width_logo) / (double)(this->logoImage.getWidth());
    int logoPlaceHeight = (screenSize.getHeight() * this->sizes.height_logo_topMargin) + this->logoImage.getHeight() * logoScale;
    int buttonWidth = leftBarWidth * this->sizes.width_button;
    int buttonHeight = screenSize.getHeight() * this->sizes.height_button;
    int buttonLogoSplitHeight = screenSize.getHeight() * this->sizes.height_button_logo_split;
    int buttonSplitHeight = screenSize.getHeight() * this->sizes.height_button_split;
    int buttonWCenter = leftBarWidth / 2;
    int buttonHPos = logoPlaceHeight + buttonLogoSplitHeight;

    //调整按钮大小位置
    this->btNewProj->setBounds(
        buttonWCenter - buttonWidth / 2, buttonHPos,
        buttonWidth, buttonHeight
    );
    this->btOpenProj->setBounds(
        buttonWCenter - buttonWidth / 2, buttonHPos + buttonHeight + buttonSplitHeight,
        buttonWidth, buttonHeight
    );
}

void SMComponent::paint(juce::Graphics& g)
{
    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);
	
    //填充背景
    g.fillAll(this->colors.background);
	
    //左侧条带宽度
    int leftBarWidth =
        (int)std::min(this->sizes.width_leftBar * screenSize.getWidth(), this->sizes.width_leftBar_max);
	
    //填充左侧条带背景
    g.setColour(this->colors.leftBar);
    g.fillRect(0, 0, leftBarWidth, this->getHeight());

    //绘制左上头图
    double logoScale = (leftBarWidth * this->scales.width_logo) / (double)(this->logoImage.getWidth());
    juce::Rectangle<int> logoRect(
        (leftBarWidth / 2.0) * (1 - this->scales.width_logo), screenSize.getHeight() * this->sizes.height_logo_topMargin,
        leftBarWidth * this->scales.width_logo, this->logoImage.getHeight() * logoScale
    );
    g.drawImageWithin(
        this->logoImage,
        logoRect.getX(), logoRect.getY(),
        logoRect.getWidth(), logoRect.getHeight(),
        juce::RectanglePlacement::Flags::centred
    );

    //计算标题栏高度
    int titleBarHeight =
        (int)std::min(screenSize.getHeight() * this->sizes.height_titleBar, this->sizes.height_titleBar_max);

    //绘标题栏高度辅助线
    //g.setColour(juce::Colours::white);
    //g.drawLine(leftBarWidth, titleBarHeight, this->getWidth(), titleBarHeight);

    //绘制标题
    g.setColour(this->colors.text_title);

    juce::Font titleFont = g.getCurrentFont();
    titleFont.setHeight(titleBarHeight * this->scales.height_title);
    g.setFont(titleFont);

    juce::String titleStr = this->tr("lb_RecentProject");
    int titleWidth = titleFont.getStringWidth(titleStr);
    juce::Rectangle<int> titleRect(
        leftBarWidth + screenSize.getWidth() * this->sizes.width_title_leftMargin, titleBarHeight / 2 * this->scales.height_title,
        titleWidth, titleBarHeight * this->scales.height_title
    );

    g.drawFittedText(
        titleStr, titleRect,
        juce::Justification::centred, 1
    );
}