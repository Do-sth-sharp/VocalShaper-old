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
	if (!(
		jmadf::LoadModule("WuChang.JMADF.LookAndFeelConfigs") &&
		jmadf::LoadModule("WuChang.JMADF.DynamicRC") &&
		jmadf::LoadModule("WuChang.JMADF.Device") &&
		jmadf::LoadModule("WuChang.JMADF.GlobalConfigs") &&
		jmadf::LoadModule("WuChang.JMADF.Translates") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.LookAndFeelFactory") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.ProjectHistory")
		)) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
			"WuChang.JMADF.LookAndFeelConfigs", "GetNumber") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
			"WuChang.JMADF.LookAndFeelConfigs", "GetColor") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
			"WuChang.JMADF.LookAndFeelConfigs", "GetString") ||
		!jmadf::CheckInterface<void>(
			"WuChang.JMADF.LookAndFeelConfigs", "Close")
		) {
		jmadf::RaiseException("@WuChang.JMADF.LookAndFeelConfigs:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC", "GetRC") ||
		!jmadf::CheckInterface<void>(
			"WuChang.JMADF.DynamicRC", "Unload")
		) {
		jmadf::RaiseException("@WuChang.JMADF.DynamicRC:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Device:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.GlobalConfigs", "GetReference") ||
		!jmadf::CheckInterface<void>(
			"WuChang.JMADF.GlobalConfigs", "Close")
		) {
		jmadf::RaiseException("@WuChang.JMADF.GlobalConfigs:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<std::function<const juce::String(const juce::String&)>&>(
			"WuChang.JMADF.Translates", "GetFunc") ||
		!jmadf::CheckInterface<void>(
			"WuChang.JMADF.Translates", "Close")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Translates:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<int, juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStartMenuButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStartMenuTextEditorLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStartMenuListBoxLAF")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.LookAndFeelFactory:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<int&>(
			"VocalSharp.VocalShaper.ProjectHistory", "GetSize") ||
		!jmadf::CheckInterface<int, juce::String&>(
			"VocalSharp.VocalShaper.ProjectHistory", "GetName") ||
		!jmadf::CheckInterface<int, juce::String&>(
			"VocalSharp.VocalShaper.ProjectHistory", "GetPath") ||
		!jmadf::CheckInterface<int, juce::String&>(
			"VocalSharp.VocalShaper.ProjectHistory", "GetTime") ||
		!jmadf::CheckInterface<int>(
			"VocalSharp.VocalShaper.ProjectHistory", "Open") ||
		!jmadf::CheckInterface<int>(
			"VocalSharp.VocalShaper.ProjectHistory", "Remove") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&>(
			"VocalSharp.VocalShaper.ProjectHistory", "Add") ||
		!jmadf::CheckInterface<const juce::String&>(
			"VocalSharp.VocalShaper.ProjectHistory", "SetFilter")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.ProjectHistory:Bad Interfaces!");
		return false;
	}

	this->smComp = std::make_unique<SMComponent>();
	if (!this->smComp) {
		jmadf::RaiseException("Can't alloc memory space for start menu!");
		return false;
	}

	auto ptrSM = this->smComp.get();
	jmadf::RegisterInterface<juce::Component*&>(
		"GetPtr",
		[ptrSM](const juce::String& caller, juce::Component*& ptr) {
			ptrSM->setCaller(caller);
			ptr = ptrSM;
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&>(
		"OpenProjectFromUrl",
		[ptrSM](const juce::String&,
			const juce::String& name, const juce::String& path) {
				ptrSM->openProjFromUrl(name, path);
		}
	);
	
	return true;
}

void VocalSharp_VocalShaper_StartMenu::destory()
{
	this->smComp = nullptr;
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