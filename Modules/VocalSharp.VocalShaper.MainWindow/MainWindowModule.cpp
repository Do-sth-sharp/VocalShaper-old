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
	
	bool result = false;
	double v1 = 0;
	bool v2 = false;
	juce::String v3;
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "testW", "testA1", v1, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, bool&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetBoolean",
		"main", "testW", "testA2", v2, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "testW", "testA3", v3, result
		);
	//this->mainWindow->setVisible(true);
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