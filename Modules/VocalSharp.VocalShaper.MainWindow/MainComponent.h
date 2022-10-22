#pragma once
#include <JuceHeader.h>

class MainComponent final
    : public juce::Component,
    public juce::FileDragAndDropTarget,
    public juce::TextDragAndDropTarget
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
    bool isOpenedProj(const juce::String& name, const juce::String& path);
    bool isSavedProj(const juce::String& name, const juce::String& path);

    void setSMVisible(bool isVisible);
    bool getSMVisible();

    void moveSplashIn(juce::Component* splash);
    void openStringFromUrl(const juce::String& str);

    bool isInterestedInTextDrag(const juce::String& text) override;
    void textDropped(const juce::String& text, int, int) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int, int) override;

    void refresh();

private:
    juce::Component* ptrStartMenu = nullptr;
    juce::Component* ptrMainUI = nullptr;
    std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
        = [](juce::Component*, juce::Rectangle<int>&) {};

    juce::SharedResourcePointer<juce::TooltipWindow> toolTip;

    juce::Component* splash = nullptr;
    juce::String projectExtension;

    bool checkStringCouldOpen(const juce::String& string);
    void stringOpen(const juce::String& string);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
