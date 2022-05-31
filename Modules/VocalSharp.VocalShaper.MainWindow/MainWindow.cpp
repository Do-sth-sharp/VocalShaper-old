#include "MainWindow.h"
#include "libJModule.h"

#if JUCE_WINDOWS
#include <Windows.h>
#include <ShlObj.h>
#endif

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
    this->centreWithSize(getWidth(), getHeight());
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
    this->setIcon(juce::ImageFileFormat::loadFrom(iconPtr, iconSize));
	
    this->getPeer()->setIcon(juce::ImageFileFormat::loadFrom(iconPtr, iconSize));
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
