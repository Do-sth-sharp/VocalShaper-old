#include "MainComponent.h"
#include <libJModule.h>

MainComponent::MainComponent()
    : Component("Main Window Central Component")
{
    //获取屏幕属性接口
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
            );

    //以下获取全局设置
    {
        juce::var* config = nullptr;
        bool ok = false;
        jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
            "WuChang.JMADF.GlobalConfigs", "GetReference",
            "config", config, ok
            );
        if (ok && (config != nullptr)) {
            if ((*config)["ProjectExtension"].isString()) {
                this->projectExtension = (*config)["ProjectExtension"].toString();
            }
        }
    }

    setSize (600, 400);

    this->addChildComponent(this->toolTip.get());

    jmadf::CallInterface<juce::Component*&>(
        "VocalSharp.VocalShaper.MainUI", "GetPtr",
        this->ptrMainUI
        );

    jmadf::CallInterface<juce::Component*&>(
        "VocalSharp.VocalShaper.StartMenu", "GetPtr",
        this->ptrStartMenu
        );
	
    if (this->ptrMainUI) {
        this->addChildComponent(this->ptrMainUI);
        this->ptrMainUI->setSize(this->getWidth(), this->getHeight());
        //this->ptrMainUI->setVisible(true);
    }

    if (this->ptrStartMenu) {
		this->addChildComponent(this->ptrStartMenu);
        this->ptrStartMenu->setSize(this->getWidth(), this->getHeight());
        this->ptrStartMenu->setVisible(true);
    }
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (juce::Graphics& g)
{
}

void MainComponent::resized()
{
    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);

    if (this->ptrMainUI) {
        this->ptrMainUI->setSize(this->getWidth(), this->getHeight());
    }
    if (this->ptrStartMenu) {
        this->ptrStartMenu->setSize(this->getWidth(), this->getHeight());
        this->ptrStartMenu->toFront(true);
    }
    if (this->splash) {
        int sizeBase = (int)(std::min(screenSize.getWidth(), screenSize.getHeight()) * 0.03);
        this->splash->centreWithSize(sizeBase * 25, sizeBase * 10);
        this->splash->toFront(true);
    }
}

bool MainComponent::newProj(const juce::String& name, const juce::String& path)
{
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
        "VocalSharp.VocalShaper.MainUI", "NewProject",
        name, path, result
        );
    if (result) {
        this->setSMVisible(false);
    }
    return result;
}

bool MainComponent::copyProj(const juce::String& name, const juce::String& path,
    const juce::String& nameSrc, const juce::String& pathSrc)
{
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
        "VocalSharp.VocalShaper.MainUI", "CopyProject",
        name, path, nameSrc, pathSrc, result
        );
    if (result) {
        this->setSMVisible(false);
    }
    return result;
}

bool MainComponent::openProj(const juce::String& name, const juce::String& path)
{
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
        "VocalSharp.VocalShaper.MainUI", "OpenProject",
        name, path, result
        );
    if (result) {
        this->setSMVisible(false);
    }
    return result;
}

void MainComponent::setSMVisible(bool isVisible)
{
    this->ptrStartMenu->setVisible(isVisible);
    this->ptrMainUI->setVisible(!isVisible);
    if (isVisible) {
        this->ptrStartMenu->grabKeyboardFocus();
    }
    else {
        this->ptrMainUI->grabKeyboardFocus();
    }
}

bool MainComponent::getSMVisible()
{
    return this->ptrStartMenu->isVisible();
}

void MainComponent::moveSplashIn(juce::Component* splash)
{
    if (splash) {
        this->splash = splash;
        this->splash->removeFromDesktop();
        this->addAndMakeVisible(splash);
        this->resized();
    }
}

void MainComponent::openStringFromUrl(const juce::String& str)
{
    this->stringOpen(str);
}

bool MainComponent::isInterestedInTextDrag(const juce::String& text)
{
    return this->checkStringCouldOpen(text);
}

void MainComponent::textDropped(const juce::String& text, int, int)
{
    this->stringOpen(text);
}

bool MainComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto& i : files) {
        if (!this->checkStringCouldOpen(i)) {
            return false;
        }
    }
    return true;
}

void MainComponent::filesDropped(const juce::StringArray& files, int, int)
{
    for (auto& i : files) {
        this->stringOpen(i);
    }
}

void MainComponent::refresh()
{
    //刷新大小
    if (this->ptrMainUI) {
        this->ptrMainUI->setSize(this->getWidth() / 2, this->getHeight() / 2);
    }
    if (this->ptrStartMenu) {
        this->ptrStartMenu->setSize(this->getWidth() / 2, this->getHeight() / 2);
        this->ptrStartMenu->toFront(true);
    }
    if (this->splash) {
        this->splash->centreWithSize(this->splash->getWidth() / 2, this->splash->getHeight() / 2);
        this->splash->toFront(true);
    }

    if (this->ptrMainUI) {
        this->ptrMainUI->setSize(this->getWidth(), this->getHeight());
    }
    if (this->ptrStartMenu) {
        this->ptrStartMenu->setSize(this->getWidth(), this->getHeight());
        this->ptrStartMenu->toFront(true);
    }
    if (this->splash) {
        this->splash->centreWithSize(this->splash->getWidth(), this->splash->getHeight());
        this->splash->toFront(true);
    }
}

bool MainComponent::checkStringCouldOpen(const juce::String& string)
{
    juce::File file(string);
    if (file.existsAsFile()) {
        juce::String extension = file.getFileExtension();
        if (extension == this->projectExtension) {
            return true;
        }
    }
    else if (file.isDirectory() && file.exists()) {
        return true;
    }
    return false;
}

void MainComponent::stringOpen(const juce::String& string)
{
    juce::File file(string);
    if (file.existsAsFile()) {
        juce::String extension = file.getFileExtension();
        if (extension == this->projectExtension) {
            jmadf::CallInterface<const juce::String&>(
                "VocalSharp.VocalShaper.StartMenu", "OpenPathFromUrl",
                file.getParentDirectory().getFullPathName()
                );
            return;
        }
    }
    else if (file.isDirectory() && file.exists()) {
        jmadf::CallInterface<const juce::String&>(
            "VocalSharp.VocalShaper.StartMenu", "OpenPathFromUrl",
            file.getFullPathName()
            );
        return;
    }
}
