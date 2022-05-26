#include "MainModule.h"

MainModule::MainModule()
	:Module()
{
	
}

MainModule::~MainModule()
{
	
}

bool MainModule::init()
{
	const char* mainWindowName = "VocalSharp.VocalShaper.MainWindow";
	if (!jmadf::LoadModule(mainWindowName)) {
		jmadf::RaiseException("@ERROR " + juce::String(mainWindowName));
		return false;
	}
	return true;
}

void MainModule::destory()
{
	jmadf::UnloadModule("VocalSharp.VocalShaper.MainWindow");
}