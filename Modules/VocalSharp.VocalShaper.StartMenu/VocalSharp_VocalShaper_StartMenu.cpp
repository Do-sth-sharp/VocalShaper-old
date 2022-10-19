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
		jmadf::LoadModule("VocalSharp.VocalShaper.ProjectHistory") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.CommandManager")
		)) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
			"WuChang.JMADF.LookAndFeelConfigs", "GetNumber") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
			"WuChang.JMADF.LookAndFeelConfigs", "GetColor") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
			"WuChang.JMADF.LookAndFeelConfigs", "GetString")
		) {
		jmadf::RaiseException("@WuChang.JMADF.LookAndFeelConfigs:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC", "GetRC")
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
			"WuChang.JMADF.GlobalConfigs", "GetReference")
		) {
		jmadf::RaiseException("@WuChang.JMADF.GlobalConfigs:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<std::function<const juce::String(const juce::String&)>&>(
			"WuChang.JMADF.Translates", "GetFunc")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Translates:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<const std::function<int()>&, juce::LookAndFeel*&>(
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
	if (
		!jmadf::CheckInterface<const juce::String&, const std::function<void(void)>&>(
			"VocalSharp.VocalShaper.CommandManager", "RegisterFunction") ||
		!jmadf::CheckInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID") ||
		!jmadf::CheckInterface<juce::ApplicationCommandManager*&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandManager")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.CommandManager:Bad Interfaces!");
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
	jmadf::RegisterInterface<const juce::String&>(
		"OpenPathFromUrl",
		[ptrSM](const juce::String&,  const juce::String& path) {
				ptrSM->openPathFromUrl(path);
		}
	);
	
	return true;
}

void VocalSharp_VocalShaper_StartMenu::destory()
{
	this->smComp = nullptr;
}