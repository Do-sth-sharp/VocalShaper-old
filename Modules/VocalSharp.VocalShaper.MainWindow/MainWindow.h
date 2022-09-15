#pragma once
#include <JuceHeader.h>
#include "MainComponent.h"

class MainWindow final : public juce::DocumentWindow
{
public:
    MainWindow(juce::String name);

    void closeButtonPressed() override;

    void resized() override;

    bool newProj(const juce::String& name, const juce::String& path);
    bool copyProj(const juce::String& name, const juce::String& path,
        const juce::String& nameSrc, const juce::String& pathSrc);
    bool openProj(const juce::String& name, const juce::String& path);

    void showStartMenu();
    void moveSplashIn(juce::Component* splash);

    void openProjFromUrl(const juce::String& name, const juce::String& path);
	
private:
    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};
    MainComponent* mComp = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};