#pragma once
#include <JuceHeader.h>

class MainComponent final : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Component* ptrStartMenu = nullptr;
    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
