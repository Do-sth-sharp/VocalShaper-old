#pragma once
#include <JuceHeader.h>
#include "MainComponent.h"

class MainWindow final : public juce::DocumentWindow
{
public:
    MainWindow(juce::String name);

    void closeButtonPressed() override;

    void resized() override;
	
private:
    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};