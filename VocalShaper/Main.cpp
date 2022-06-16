#include <JuceHeader.h>
#include "Splash.h"
#include "JMADF/JMADF.h"
#include "JMADF/libJModule/Utils.h"

class VocalShaperApplication final : public juce::JUCEApplication
{
public:
    VocalShaperApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return false; }

    void initialise (const juce::String& /*commandLine*/) override
    {
        this->splash.reset(new Splash(this->getApplicationVersion(), jmadf::getComplieTime(__DATE__, __TIME__)));
        const juce::Rectangle<int>& displayArea =
            juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->totalArea;
        int sizeBase = (int)(std::min(displayArea.getWidth(), displayArea.getHeight()) * 0.03);
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
		
        this->splash->showMessage("Loading base modules...");
        if (!JMADF::loadFromLoader("WuChang.JMADF.DynamicRC")) {
            juce::String exMes = JMADF::getException();
            JMADF::clearException();
            juce::AlertWindow::showMessageBox(
                juce::MessageBoxIconType::WarningIcon, "Base Module Fatal Error",
                exMes, juce::String(),
                this->splash.get()
            );
            quit();
        }
        if (!JMADF::loadFromLoader("WuChang.JMADF.LookAndFeelConfigs")) {
            juce::String exMes = JMADF::getException();
            JMADF::clearException();
            juce::AlertWindow::showMessageBox(
                juce::MessageBoxIconType::WarningIcon, "Base Module Fatal Error",
                exMes, juce::String(),
                this->splash.get()
            );
            quit();
        }
		
        this->splash->showMessage("Loading main module...");
        if (!JMADF::loadFromLoader("VocalSharp.VocalShaper.Main")) {
            juce::String exMes = JMADF::getException();
            JMADF::clearException();
            juce::AlertWindow::showMessageBox(
                juce::MessageBoxIconType::WarningIcon, "Main Module Fatal Error",
                exMes, juce::String(),
                this->splash.get()
            );
            quit();
        }
		
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
