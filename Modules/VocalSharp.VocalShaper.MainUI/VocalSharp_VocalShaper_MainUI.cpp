#include "VocalSharp_VocalShaper_MainUI.h"
#include <libJModule.h>

VocalSharp_VocalShaper_MainUI::VocalSharp_VocalShaper_MainUI()
	:Module()
{
	
}

VocalSharp_VocalShaper_MainUI::~VocalSharp_VocalShaper_MainUI()
{
	
}

bool VocalSharp_VocalShaper_MainUI::init()
{
	if (!(
		jmadf::LoadModule("WuChang.JMADF.LookAndFeelConfigs") &&
		jmadf::LoadModule("WuChang.JMADF.DynamicRC") &&
		jmadf::LoadModule("WuChang.JMADF.Device") &&
		jmadf::LoadModule("WuChang.JMADF.GlobalConfigs") &&
		jmadf::LoadModule("WuChang.JMADF.Translates") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.LookAndFeelFactory")
		)) {
		return false;
	}

	this->UIComp = std::make_unique<UIComponent>();
	if (!this->UIComp) {
		jmadf::RaiseException("Can't alloc memory space for main UI!");
		return false;
	}

	auto ptrUI = this->UIComp.get();
	jmadf::RegisterInterface<juce::Component*&>(
		"GetPtr",
		[ptrUI](const juce::String& caller, juce::Component*& ptr) {
			ptrUI->setCaller(caller);
			ptr = ptrUI;
		}
	);

	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"NewProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = true;
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
		"CopyProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path,
			const juce::String& nameSrc, const juce::String& pathSrc, bool& result)
		{
			result = true;
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"OpenProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = true;
		}
	);

	jmadf::RegisterInterface<bool&>(
		"CloseIsAvailable",
		[this](const juce::String& caller, bool& result)
		{
			result = true;
		}
	);

	return true;
}

void VocalSharp_VocalShaper_MainUI::destory()
{
	this->UIComp = nullptr;
	jmadf::CallInterface<void>(
		"WuChang.JMADF.GlobalConfigs", "Close"
		);
	jmadf::CallInterface<void>(
		"WuChang.JMADF.Translates", "Close"
		);
	jmadf::CallInterface<void>(
		"WuChang.JMADF.LookAndFeelConfigs", "Close"
		);
	jmadf::CallInterface<void>(
		"WuChang.JMADF.DynamicRC",
		"Unload"
		);
}