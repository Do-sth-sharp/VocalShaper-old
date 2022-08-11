#include "MainWindow.h"
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

    this->setUsingNativeTitleBar(true);
    this->setContentOwned(new MainComponent(), true);

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
    //mainLAF.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0));

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
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
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
