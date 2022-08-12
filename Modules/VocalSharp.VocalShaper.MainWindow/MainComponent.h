#pragma once
#include <JuceHeader.h>

class MainComponent final : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    bool newProj(const juce::String& name, const juce::String& path);
    bool copyProj(const juce::String& name, const juce::String& path,
        const juce::String& nameSrc, const juce::String& pathSrc);
    bool openProj(const juce::String& name, const juce::String& path);

private:
    juce::Component* ptrStartMenu = nullptr;
    juce::Component* ptrMainUI = nullptr;
    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
