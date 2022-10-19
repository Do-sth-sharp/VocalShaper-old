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

    vocalshaper::AdsorbState getAdsorb();
    void setAdsorb(vocalshaper::AdsorbState state);
    vocalshaper::GridState getGrid();
    void setGrid(vocalshaper::GridState state);

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

    void initCommandID();
    void initCommandFunction();
    void initCommandFlagHook();

    vocalshaper::AdsorbState adsorbState = vocalshaper::AdsorbState::Adsorb1Beat;
    vocalshaper::GridState gridState = vocalshaper::GridState::Grid1Beat;

    juce::ApplicationCommandManager* commandManager = nullptr;

    juce::Component* ptrEditor = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIComponent)
};
