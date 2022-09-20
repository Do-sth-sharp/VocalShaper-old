#pragma once
#include <JuceHeader.h>
#include "TabComponent.h"
#include "PlayBar.h"

class UIComponent final : public juce::Component
{
public:
    UIComponent();
    ~UIComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setCaller(const juce::String& caller);

    bool newProj(const juce::String& name, const juce::String& path);
    bool copyProj(const juce::String& name, const juce::String& path,
        const juce::String& nameSrc, const juce::String& pathSrc);
    bool openProj(const juce::String& name, const juce::String& path);
    bool wannaClose();

private:
    struct Size final
    {
        double height_tabBar;
        double height_playBar;
    }sizes;

    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};

    std::unique_ptr<TabComponent> tabBar;
    std::unique_ptr<PlayBar> playBar;

    juce::ApplicationCommandManager* commandManager = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIComponent)
};
