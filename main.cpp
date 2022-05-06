#include <JuceHeader.h>
#include "MainWindow.h"
#include "JMADF/JMADF.h"

class VocalShaperApplication final : public juce::JUCEApplication
{
public:
    VocalShaperApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return false; }

    void initialise (const juce::String& /*commandLine*/) override
    {
        JMADF::init(
            juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
            .getParentDirectory().getFullPathName() + "/modules/",
            "{08338208-B752-4A90-9C73-4163152D6818}"
        );
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
        JMADF::destroy();
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const juce::String& /*commandLine*/) override
    {
    }

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (VocalShaperApplication)
