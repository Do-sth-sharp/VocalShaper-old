#include <JuceHeader.h>
#include "Splash.h"
#include "JMADF/JMADF.h"
#include "JMADF/libJModule/Utils.h"
#include <libVocalShaper.h>

class VocalShaperApplication final : public juce::JUCEApplication
{
public:
    VocalShaperApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return false; }

    void initialise (const juce::String& commandLine) override
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

        //准备初始化关键值
        auto mesManager = juce::MessageManager::getInstance();
        auto splash = this->splash.get();
        
        if (!mesManager)
        {
            juce::JUCEApplication::quit();
            return;
        }

        //建立初始化方法
        auto initAppFunc = [mesManager, splash, commandLine] {
            //符号值
            static bool asyncErrorFlag = false;
            static bool asyncOpenGLFlag = false;

            static std::queue<std::function<void(void)>> asyncTaskList;

            //捕获值
            auto& errorFlag = asyncErrorFlag;
            auto& openGLFlag = asyncOpenGLFlag;
            auto& taskList = asyncTaskList;

            //添加列表任务
            auto addTaskFunc = [&taskList, mesManager](std::function<void(void)> func) {
                taskList.push([&taskList, mesManager, func] {
                    func();
                    if (!taskList.empty()) {
                        std::function<void(void)> next = std::move(taskList.front());
                        taskList.pop();
                        mesManager->callAsync(next);
                    }
                    });
            };

            addTaskFunc(
                [splash, &errorFlag] {
                    if (errorFlag) { return; }
                    splash->showMessage("Getting module list..."); });
            addTaskFunc(
                [&errorFlag] {
                    if (errorFlag) { return; }
                    JMADF::init(
                        juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
                        .getParentDirectory().getFullPathName() + "/modules",
                        "{08338208-B752-4A90-9C73-4163152D6818}"
                    );
                });


            addTaskFunc(
                [splash, &errorFlag] {
                    if (errorFlag) { return; }
                    splash->showMessage("Loading base modules..."); });
            addTaskFunc(
                [splash, &errorFlag] {
                    if (errorFlag) { return; }
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
                            splash
                        );
                        juce::JUCEApplication::quit();
                        errorFlag = true;
                    }});

            addTaskFunc(
                [&openGLFlag, &errorFlag] {
                    if (errorFlag) { return; }
                    openGLFlag = JMADF::loadFromLoader("WuChang.JMADF.OpenGLComponentRender");
                    if (!openGLFlag) {
                        JMADF::clearException();
                    }
                });

            addTaskFunc(
                [splash, &errorFlag] {
                    if (errorFlag) { return; }
                    splash->showMessage("Loading main module..."); });
            addTaskFunc(
                [splash, &errorFlag] {
                    if (errorFlag) { return; }
                    if (!JMADF::loadFromLoader("VocalSharp.VocalShaper.Main")) {
                        juce::String exMes = JMADF::getException();
                        JMADF::clearException();
                        juce::AlertWindow::showMessageBox(
                            juce::MessageBoxIconType::WarningIcon, "Main Module Fatal Error",
                            exMes, juce::String(),
                            splash
                        );
                        juce::JUCEApplication::quit();
                        errorFlag = true;
                    }
                });

            addTaskFunc(
                [splash, &errorFlag] {
                    if (errorFlag) { return; }
                    if (!InterfaceDao<juce::Component*>::checkFromLoader(
                        "VocalSharp.VocalShaper.Main", "MoveToMainWindow") ||
                        !InterfaceDao<const juce::String&>::checkFromLoader(
                            "VocalSharp.VocalShaper.Main", "ParseCommand")) {
                        juce::AlertWindow::showMessageBox(
                            juce::MessageBoxIconType::WarningIcon, "Main Module Fatal Error",
                            "Bad Interfaces!", juce::String(),
                            splash
                        );
                        juce::JUCEApplication::quit();
                        errorFlag = true;
                    }
                });

            addTaskFunc(
                [splash, &errorFlag] {
                    if (errorFlag) { return; }
                    InterfaceDao<juce::Component*>::callFromLoader(
                        "VocalSharp.VocalShaper.Main", "MoveToMainWindow",
                        splash
                    );
                });

            addTaskFunc(
                [splash, &errorFlag] {
                    if (errorFlag) { return; }
                    splash->ready(); });
            addTaskFunc(
                [splash, &errorFlag, &openGLFlag] {
                    if (errorFlag) { return; }
                    if (openGLFlag) {
                        splash->showMessage("Ready with OpenGL.");
                    }
                    else {
                        splash->showMessage("Ready.");
                    }
                });

            addTaskFunc(
                [commandLine, &errorFlag] {
                    if (errorFlag) { return; }
                    InterfaceDao<const juce::String&>::callFromLoader(
                        "VocalSharp.VocalShaper.Main", "ParseCommand",
                        commandLine
                    );
                });

            //执行首个task
            if (!taskList.empty()) {
                std::function<void(void)> first = std::move(taskList.front());
                taskList.pop();
                mesManager->callAsync(first);
            }
        };
        
        //执行初始化
        mesManager->callAsync(initAppFunc);
    }

    void shutdown() override
    {
        this->splash = nullptr;
        JMADF::destroy();
        vocalshaper::files::vsp3::shutDownProtobufLibrary();
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        InterfaceDao<const juce::String&>::callFromLoader(
            "VocalSharp.VocalShaper.Main", "ParseCommand",
            commandLine
        );
    }
	
private:
    std::unique_ptr<Splash> splash;
};

START_JUCE_APPLICATION (VocalShaperApplication)
