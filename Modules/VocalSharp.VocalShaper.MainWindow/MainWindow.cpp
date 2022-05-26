#include "MainWindow.h"

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
    this->setVisible(true);
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
