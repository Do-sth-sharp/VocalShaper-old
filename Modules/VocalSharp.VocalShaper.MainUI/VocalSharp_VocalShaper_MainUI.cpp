#include "VocalSharp_VocalShaper_MainUI.h"
#include <libJModule.h>
#include <libVocalShaper.h>

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
		jmadf::LoadModule("VocalSharp.VocalShaper.LookAndFeelFactory") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.ProjectHub") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.MainMenu") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.CommandManager") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.MainEditor") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.CallbackReactor")
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
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetTabCloseButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetMainMenuButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetPlayButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetPlayTextButtonLAF")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.LookAndFeelFactory:Bad Interfaces!");
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
			"VocalSharp.VocalShaper.ProjectHub", "CloseProj") ||
		!jmadf::CheckInterface<const std::function<void(const vocalshaper::ProjectProxy*)>&>(
			"VocalSharp.VocalShaper.ProjectHub", "AddNotice") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.ProjectHub", "IsOpened") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.ProjectHub", "IsSaved")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.ProjectHub:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<juce::PopupMenu&>(
			"VocalSharp.VocalShaper.MainMenu", "GetMenu") ||
		!jmadf::CheckInterface<int>(
			"VocalSharp.VocalShaper.MainMenu", "MenuActived")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.MainMenu:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, const std::function<void(void)>&>(
			"VocalSharp.VocalShaper.CommandManager", "RegisterFunction") ||
		!jmadf::CheckInterface<const juce::String&, const std::function<int(void)>&>(
			"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook") ||
		!jmadf::CheckInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID") ||
		!jmadf::CheckInterface<juce::ApplicationCommandManager*&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandManager")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.CommandManager:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<juce::Component*&>(
			"VocalSharp.VocalShaper.MainEditor", "GetPtr") ||
		!jmadf::CheckInterface<vocalshaper::AdsorbState>(
			"VocalSharp.VocalShaper.MainEditor", "SetAdsorb") ||
		!jmadf::CheckInterface<vocalshaper::GridState>(
			"VocalSharp.VocalShaper.MainEditor", "SetGrid")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.MainEditor:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<const vocalshaper::ProjectProxy::SaveCallbackFunc&>(
			"VocalSharp.VocalShaper.CallbackReactor", "AddSaveCallback") ||
		!jmadf::CheckInterface<const vocalshaper::ProjectProxy::CloseCallbackFunc&>(
			"VocalSharp.VocalShaper.CallbackReactor", "AddCloseCallback") ||
		!jmadf::CheckInterface<const vocalshaper::EventProcesser::EventHandleFunc&>(
			"VocalSharp.VocalShaper.CallbackReactor", "AddEventHandler") ||
		!jmadf::CheckInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
			"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.CallbackReactor:Bad Interfaces!");
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
		[ptrUI](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = ptrUI->newProj(name, path);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
		"CopyProject",
		[ptrUI](const juce::String& caller,
			const juce::String& name, const juce::String& path,
			const juce::String& nameSrc, const juce::String& pathSrc, bool& result)
		{
			result = ptrUI->copyProj(name, path, nameSrc, pathSrc);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"OpenProject",
		[ptrUI](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = ptrUI->openProj(name, path);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"IsOpenedProj",
		[ptrUI](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = ptrUI->isOpenedProj(name, path);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"IsSavedProj",
		[ptrUI](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = ptrUI->isSavedProj(name, path);
		}
	);

	jmadf::RegisterInterface<bool&>(
		"CloseIsAvailable",
		[ptrUI](const juce::String& caller, bool& result)
		{
			result = ptrUI->wannaClose();
		}
	);

	return true;
}

void VocalSharp_VocalShaper_MainUI::destory()
{
	this->UIComp = nullptr;
}