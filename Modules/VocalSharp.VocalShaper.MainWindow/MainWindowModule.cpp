#include "MainWindowModule.h"
#include <libJModule.h>

MainWindowModule::MainWindowModule()
	:Module()
{
	
}

MainWindowModule::~MainWindowModule()
{
	
}

bool MainWindowModule::init()
{
	jmadf::RegisterInterface<juce::DocumentWindow*&>(
		"GetMainWindowPtr",
		[this](const juce::String& caller, juce::DocumentWindow*& ptr)
		{
			ptr = this->mainWindow.get();
		}
	);
	
	this->mainWindow = std::make_unique<MainWindow>("VocalShaper");
	if (!this->mainWindow) {
		jmadf::RaiseException("Can't alloc memory space for main window!");
		return false;
	}
	
	this->mainWindow->setVisible(true);
	return true;
}

void MainWindowModule::destory()
{
	this->mainWindow = nullptr;
	jmadf::CallInterface<void>(
		"WuChang.JMADF.DynamicRC",
		"Unload"
		);
}