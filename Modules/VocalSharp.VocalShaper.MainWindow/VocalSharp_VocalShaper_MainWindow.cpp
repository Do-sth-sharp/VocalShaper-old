#include "VocalSharp_VocalShaper_MainWindow.h"
#include <libJModule.h>

VocalSharp_VocalShaper_MainWindow::VocalSharp_VocalShaper_MainWindow()
	:Module()
{
	
}

VocalSharp_VocalShaper_MainWindow::~VocalSharp_VocalShaper_MainWindow()
{
	
}

bool VocalSharp_VocalShaper_MainWindow::init()
{
	jmadf::LoadModule("VocalSharp.VocalShaper.StartMenu");
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
	return true;
}

void VocalSharp_VocalShaper_MainWindow::destory()
{
	this->mainWindow = nullptr;
	jmadf::CallInterface<void>(
		"WuChang.JMADF.LookAndFeelConfigs", "Close"
		);
	jmadf::CallInterface<void>(
		"WuChang.JMADF.DynamicRC",
		"Unload"
		);
	jmadf::UnloadModule("VocalSharp.VocalShaper.StartMenu");
}