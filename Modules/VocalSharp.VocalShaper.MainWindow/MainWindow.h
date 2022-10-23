#pragma once
#include <JuceHeader.h>
#include "MainComponent.h"
#include <libVocalShaper.h>

class MainWindow final : public juce::DocumentWindow
{
public:
    MainWindow(juce::String name);

    void closeButtonPressed() override;

    void moved() override;
    void parentSizeChanged() override;

    bool newProj(const juce::String& name, const juce::String& path);
    bool copyProj(const juce::String& name, const juce::String& path,
        const juce::String& nameSrc, const juce::String& pathSrc);
    bool openProj(const juce::String& name, const juce::String& path);
    bool isOpenedProj(const juce::String& name, const juce::String& path);
    bool isSavedProj(const juce::String& name, const juce::String& path);

    void showStartMenu();
    void moveSplashIn(juce::Component* splash);

    void openStringFromUrl(const juce::String& str);

    void closeEditor();

    void setProject(const vocalshaper::ProjectProxy* project);
    void listenActions(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
    void listenSaved(const vocalshaper::ProjectProxy* proj);
	
private:
    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};

    MainComponent* mComp = nullptr;
    int closeEditorCommandID = -1;
    juce::ApplicationCommandManager* commandManager = nullptr;
    const juce::Displays::Display* displayTemp = nullptr;

    const vocalshaper::ProjectProxy* project = nullptr;
    juce::ReadWriteLock projLock;

    void refreshTitle();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};