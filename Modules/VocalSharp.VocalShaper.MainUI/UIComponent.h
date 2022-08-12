#pragma once
#include <JuceHeader.h>

class UIComponent final : public juce::Component
{
public:
    UIComponent();
    ~UIComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setCaller(const juce::String& caller);

private:
    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};

    juce::String caller;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIComponent)
};
