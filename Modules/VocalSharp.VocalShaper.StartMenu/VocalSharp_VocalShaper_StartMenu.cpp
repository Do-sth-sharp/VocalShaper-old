#include "VocalSharp_VocalShaper_StartMenu.h"

VocalSharp_VocalShaper_StartMenu::VocalSharp_VocalShaper_StartMenu()
	:Module()
{
	
}

VocalSharp_VocalShaper_StartMenu::~VocalSharp_VocalShaper_StartMenu()
{
	
}

bool VocalSharp_VocalShaper_StartMenu::init()
{
	jmadf::LoadModule("WuChang.JMADF.LookAndFeelConfigs");
	jmadf::LoadModule("WuChang.JMADF.DynamicRC");
	jmadf::LoadModule("WuChang.JMADF.Device");

	this->smComp = std::make_unique<SMComponent>();
	if (!this->smComp) {
		jmadf::RaiseException("Can't alloc memory space for start menu!");
		return false;
	}

	juce::Component* ptrSM = this->smComp.get();
	jmadf::RegisterInterface<juce::Component*&>(
		"GetPtr",
		[ptrSM](const juce::String& caller, juce::Component*& ptr) {
			if (caller == "VocalSharp.VocalShaper.MainWindow") {
				ptr = ptrSM;
			}
		}
	);
	
	return true;
}

void VocalSharp_VocalShaper_StartMenu::destory()
{
	this->smComp = nullptr;
	jmadf::CallInterface<void>(
		"WuChang.JMADF.LookAndFeelConfigs", "Close"
		);
	jmadf::CallInterface<void>(
		"WuChang.JMADF.DynamicRC",
		"Unload"
		);	
}