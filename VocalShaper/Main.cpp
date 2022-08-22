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
        juce::Rectangle<int> displayArea(0, 0, 1920, 1080);
        if (auto display = 
            juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()) {
            displayArea = display->totalArea;
            int sizeBase = (int)(std::min(displayArea.getWidth(), displayArea.getHeight()) * 0.03);
            this->splash->centreWithSize(sizeBase * 25, sizeBase * 10);
        }
        else {
            int sizeBase = (int)(std::min(displayArea.getWidth(), displayArea.getHeight()) * 0.03);
            this->splash->setSize(sizeBase * 25, sizeBase * 10);
        }
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
        if (!(JMADF::loadFromLoader("WuChang.JMADF.DynamicRC") &&
            JMADF::loadFromLoader("WuChang.JMADF.LookAndFeelConfigs") &&
            JMADF::loadFromLoader("WuChang.JMADF.Device") &&
            JMADF::loadFromLoader("WuChang.JMADF.Configs") &&
            JMADF::loadFromLoader("WuChang.JMADF.GlobalConfigs") &&
            JMADF::loadFromLoader("WuChang.JMADF.Translates")
            )) {
            juce::String exMes = JMADF::getException();
            JMADF::clearException();
            juce::AlertWindow::showMessageBox(
                juce::MessageBoxIconType::WarningIcon, "Base Module Fatal Error",
                exMes, juce::String(),
                this->splash.get()
            );
            quit();
            return;
        }
		
        bool openglLoaded = JMADF::loadFromLoader("WuChang.JMADF.OpenGLComponentRender");
        if (!openglLoaded) {
            JMADF::clearException();
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
            return;
        }
		
        InterfaceDao<juce::Component*>::callFromLoader(
            "VocalSharp.VocalShaper.Main", "MoveToMainWindow",
            this->splash.get()
        );
        this->splash->ready();
        if (openglLoaded) {
            this->splash->showMessage("Ready with OpenGL.");
        }
        else {
            this->splash->showMessage("Ready.");
        }
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
