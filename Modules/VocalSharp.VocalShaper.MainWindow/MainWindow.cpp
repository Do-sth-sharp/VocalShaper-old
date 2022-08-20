﻿#include "MainWindow.h"
#include "libJModule.h"

MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name,
        juce::Desktop::getInstance().getDefaultLookAndFeel()
        .findColour(juce::ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons)
{
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
            );

    this->mComp = new MainComponent();
    this->setUsingNativeTitleBar(true);
    this->setContentOwned(this->mComp, true);

#if JUCE_IOS || JUCE_ANDROID
    this->setFullScreen(true);
#else
    this->setResizable(true, true);
    this->centreWithSize(this->getWidth(), this->getHeight());
    this->setFullScreen(true);
#endif

    //设置窗口图标
    size_t iconSize = 0;
    void* iconPtr = nullptr;
    juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
        .getParentDirectory().getFullPathName() + "/rc/logo.png";
	
    jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
        "WuChang.JMADF.DynamicRC",
        "GetRC",
        iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
        );

    const juce::Image& iconImage = juce::ImageFileFormat::loadFrom(iconPtr, iconSize);
    this->setIcon(iconImage);
	
    this->getPeer()->setIcon(iconImage);

    //设置主窗口默认背景色
    juce::Colour cBackground;
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background", cBackground, result
        );
    if (result) {
        this->setBackgroundColour(cBackground);
    }

    //设置默认提示框样式
    juce::Colour cBackgroundAlert, cTextAlert, cOutlineAlert;
    juce::Colour cBackgroundAlertButton, cTextAlertButton;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-alert", cBackgroundAlert, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-alert", cTextAlert, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "outline-alert", cOutlineAlert, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-alert-button", cBackgroundAlertButton, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-alert-button", cTextAlertButton, result
        );

    auto& mainLAF = juce::LookAndFeel::getDefaultLookAndFeel();
    mainLAF.setColour(juce::AlertWindow::ColourIds::backgroundColourId, cBackgroundAlert);
    mainLAF.setColour(juce::AlertWindow::ColourIds::textColourId, cTextAlert);
    mainLAF.setColour(juce::AlertWindow::ColourIds::outlineColourId, cOutlineAlert);
    mainLAF.setColour(juce::TextButton::ColourIds::buttonColourId, cBackgroundAlertButton);
    mainLAF.setColour(juce::TextButton::ColourIds::buttonOnColourId, cBackgroundAlertButton);
    mainLAF.setColour(juce::TextButton::ColourIds::textColourOffId, cTextAlertButton);
    mainLAF.setColour(juce::TextButton::ColourIds::textColourOnId, cTextAlertButton);
    mainLAF.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0));

    //设置默认菜单样式
    juce::Colour cBackgroundMenu, cTextMenu, cHeaderMenu, cBackgroundMenuHighlight, cTextMenuHighlight;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-menu", cBackgroundMenu, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-menu", cTextMenu, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "header-menu", cHeaderMenu, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-menu-highlight", cBackgroundMenuHighlight, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-menu-highlight", cTextMenuHighlight, result
        );

    mainLAF.setColour(juce::PopupMenu::ColourIds::backgroundColourId, cBackgroundMenu);
    mainLAF.setColour(juce::PopupMenu::ColourIds::textColourId, cTextMenu);
    mainLAF.setColour(juce::PopupMenu::ColourIds::headerTextColourId, cHeaderMenu);
    mainLAF.setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, cBackgroundMenuHighlight);
    mainLAF.setColour(juce::PopupMenu::ColourIds::highlightedTextColourId, cTextMenuHighlight);

    //设置默认弹出提示样式
    juce::Colour cBackgroundToolTip, cTextToolTip;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-toolTip", cBackgroundToolTip, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-toolTip", cTextToolTip, result
        );

    mainLAF.setColour(juce::TooltipWindow::ColourIds::backgroundColourId, cBackgroundToolTip);
    mainLAF.setColour(juce::TooltipWindow::ColourIds::textColourId, cTextToolTip);
    mainLAF.setColour(juce::TooltipWindow::ColourIds::outlineColourId, cBackgroundToolTip);
}

void MainWindow::closeButtonPressed()
{
    bool SMIsVisible = this->mComp->getSMVisible();
    this->mComp->setSMVisible(false);

    bool closeOk = false;
    jmadf::CallInterface<bool&>(
        "VocalSharp.VocalShaper.MainUI", "CloseIsAvailable",
        closeOk
        );
    if (closeOk) {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }
    else {
        this->mComp->setSMVisible(SMIsVisible);
    }
}

void MainWindow::resized()
{
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);
    this->setResizeLimits(
        screenSize.getWidth() * 0.5, screenSize.getHeight() * 0.5,
        screenSize.getWidth() * 2, screenSize.getHeight() * 2
    );
    this->DocumentWindow::resized();
}

bool MainWindow::newProj(const juce::String& name, const juce::String& path)
{
    return this->mComp->newProj(name, path);
}

bool MainWindow::copyProj(const juce::String& name, const juce::String& path,
    const juce::String& nameSrc, const juce::String& pathSrc)
{
    return this->mComp->copyProj(name, path, nameSrc, pathSrc);
}

bool MainWindow::openProj(const juce::String& name, const juce::String& path)
{
    return this->mComp->openProj(name, path);
}

void MainWindow::showStartMenu()
{
    this->mComp->setSMVisible(true);
}
