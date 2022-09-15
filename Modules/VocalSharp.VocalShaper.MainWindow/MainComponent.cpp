#include "MainComponent.h"
#include <libJModule.h>

MainComponent::MainComponent()
    : Component("Main Window Central Component")
{
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
            );

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
        this->ptrMainUI->setVisible(true);
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

bool MainComponent::newProj(const juce::String& name, const juce::String& path)
{
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
        "VocalSharp.VocalShaper.MainUI", "NewProject",
        name, path, result
        );
    if (result) {
        this->ptrStartMenu->setVisible(false);
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
        this->ptrStartMenu->setVisible(false);
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
        this->ptrStartMenu->setVisible(false);
    }
    return result;
}

void MainComponent::setSMVisible(bool isVisible)
{
    this->ptrStartMenu->setVisible(isVisible);
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
        this->splash->centreWithSize(this->splash->getWidth(), this->splash->getHeight());
        this->toFront(true);
    }
}

void MainComponent::openProjFromUrl(const juce::String& name, const juce::String& path)
{
    jmadf::CallInterface<const juce::String&, const juce::String&>(
        "VocalSharp.VocalShaper.StartMenu", "OpenProjectFromUrl",
        name, path
        );
}