#include "MainWindow.h"
#include "libJModule.h"

MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name,
        juce::Desktop::getInstance().getDefaultLookAndFeel()
        .findColour(juce::ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons)
{
    this->setUsingNativeTitleBar(true);
    this->setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
    this->setFullScreen(true);
#else
    this->setResizable(true, true);
    this->centreWithSize(this->getWidth(), this->getHeight());
    this->setFullScreen(true);
#endif

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

    juce::Colour cBackground;
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background", cBackground, result
        );
    if (result) {
        this->setBackgroundColour(cBackground);
    }
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainWindow::resized()
{
    juce::Rectangle<int> screenSize;
    jmadf::CallInterface<juce::Component*, juce::Rectangle<int>&>(
        "WuChang.JMADF.Device", "GetScreenSize",
        this, screenSize
        );
    this->setResizeLimits(
        screenSize.getWidth() * 0.5, screenSize.getHeight() * 0.5,
        screenSize.getWidth() * 2, screenSize.getHeight() * 2
    );
    this->DocumentWindow::resized();
}
