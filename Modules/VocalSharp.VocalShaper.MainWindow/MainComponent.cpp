#include "MainComponent.h"
#include <libJModule.h>

MainComponent::MainComponent()
    : Component("Main Window Central Component")
{
    setSize (600, 400);
    jmadf::CallInterface<juce::Component*&>(
        "VocalSharp.VocalShaper.StartMenu", "GetPtr",
        this->ptrStartMenu
        );
	
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
    if (this->ptrStartMenu) {
        this->ptrStartMenu->setSize(this->getWidth(), this->getHeight());
    }
}
