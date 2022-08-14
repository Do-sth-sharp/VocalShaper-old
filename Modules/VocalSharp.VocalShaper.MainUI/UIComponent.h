#pragma once
#include <JuceHeader.h>
#include "TabComponent.h"

class UIComponent final : public juce::Component
{
public:
    UIComponent();
    ~UIComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setCaller(const juce::String& caller);

private:
    struct Size final
    {
        double height_tabBar;
    }sizes;

    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};

    juce::String caller;

    std::unique_ptr<TabComponent> tabBar;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIComponent)
};
