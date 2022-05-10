#include <JuceHeader.h>
#include "Splash.h"
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
        this->splash.reset(new Splash(this->getApplicationVersion()));
        const juce::Rectangle<int>& displayArea =
            juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->totalArea;
        int sizeBase = std::min(displayArea.getWidth(), displayArea.getHeight()) * 0.03;
        this->splash->centreWithSize(sizeBase * 25, sizeBase * 10);
		this->splash->setVisible(true);
        this->splash->addToDesktop(juce::ComponentPeer::windowHasDropShadow);
        this->splash->toFront(true);
		
        this->splash->showMessage("Getting module list...");
        JMADF::init(
            juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
            .getParentDirectory().getFullPathName() + "/modules",
            "{08338208-B752-4A90-9C73-4163152D6818}"
        );
        this->splash->showMessage("Loading main module...");
        JMADF::load("VocalSharp.VocalShaper.Main");
        this->splash->ready();
        this->splash->showMessage("Ready.");
    }

    void shutdown() override
    {
        this->splash = nullptr;
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
    std::unique_ptr<Splash> splash;
};

START_JUCE_APPLICATION (VocalShaperApplication)
