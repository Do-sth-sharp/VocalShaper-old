#include "VocalSharp_VocalShaper_MainMenu.h"
#include <libJModule.h>
#include <libVocalShaper.h>
#include "MainMenu.h"

VocalSharp_VocalShaper_MainMenu::VocalSharp_VocalShaper_MainMenu()
	:Module()
{
	
}

VocalSharp_VocalShaper_MainMenu::~VocalSharp_VocalShaper_MainMenu()
{
	
}

bool VocalSharp_VocalShaper_MainMenu::init()
{
	if (!(
		jmadf::LoadModule("WuChang.JMADF.GlobalConfigs") &&
		jmadf::LoadModule("WuChang.JMADF.Translates") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.ProjectHub")
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
		!jmadf::CheckInterface<std::function<const juce::String(const juce::String&)>&>(
			"WuChang.JMADF.Translates", "GetFunc") ||
		!jmadf::CheckInterface<void>(
			"WuChang.JMADF.Translates", "Close")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Translates:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<int, vocalshaper::ProjectProxy*&>(
			"VocalSharp.VocalShaper.ProjectHub", "GetPtr") ||
		!jmadf::CheckInterface<int&>(
			"VocalSharp.VocalShaper.ProjectHub", "GetCurrent") ||
		!jmadf::CheckInterface<int&>(
			"VocalSharp.VocalShaper.ProjectHub", "GetSize") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.ProjectHub", "NewProject") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.ProjectHub", "CopyProject") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.ProjectHub", "OpenProject") ||
		!jmadf::CheckInterface<int>(
			"VocalSharp.VocalShaper.ProjectHub", "SetCurrent") ||
		!jmadf::CheckInterface<int>(
			"VocalSharp.VocalShaper.ProjectHub", "SetCurrentAndToFront") ||
		!jmadf::CheckInterface<int, bool&>(
			"VocalSharp.VocalShaper.ProjectHub", "CheckForClose") ||
		!jmadf::CheckInterface<int, bool&>(
			"VocalSharp.VocalShaper.ProjectHub", "SaveProj") ||
		!jmadf::CheckInterface<int>(
			"VocalSharp.VocalShaper.ProjectHub", "CloseProj")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.ProjectHub:Bad Interfaces!");
		return false;
	}

	jmadf::RegisterInterface<juce::PopupMenu&>(
		"GetMenu",
		[](const juce::String&, juce::PopupMenu& menu) {
			menu = MainMenu::create();
		}
	);
	jmadf::RegisterInterface<int>(
		"MenuActived",
		[](const juce::String&, int id) {
			MainMenu::active(id);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_MainMenu::destory()
{
	jmadf::CallInterface<void>(
		"WuChang.JMADF.GlobalConfigs", "Close"
		);
	jmadf::CallInterface<void>(
		"WuChang.JMADF.Translates", "Close"
		);
}