#include "VocalSharp_VocalShaper_MainEditor.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_MainEditor::VocalSharp_VocalShaper_MainEditor()
	:Module()
{
	
}

VocalSharp_VocalShaper_MainEditor::~VocalSharp_VocalShaper_MainEditor()
{
	
}

bool VocalSharp_VocalShaper_MainEditor::init()
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
		jmadf::LoadModule("VocalSharp.VocalShaper.ClipBoard")
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
		!jmadf::CheckInterface<juce::LookAndFeel*&, juce::Colour>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStretchableBarLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetToolButtonLAF")
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
		!jmadf::CheckInterface<void>(
			"VocalSharp.VocalShaper.ProjectHub", "Release")
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
			"VocalSharp.VocalShaper.CommandManager", "GetCommandManager") ||
		!jmadf::CheckInterface<void>(
			"VocalSharp.VocalShaper.CommandManager", "Close")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.CommandManager:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<juce::OwnedArray<::vocalshaper::SerializableProjectStructure>&&>(
			"VocalSharp.VocalShaper.ClipBoard", "CopyAndCut") ||
		!jmadf::CheckInterface<juce::OwnedArray<::vocalshaper::SerializableProjectStructure>&>(
			"VocalSharp.VocalShaper.ClipBoard", "Paste") ||
		!jmadf::CheckInterface<juce::OwnedArray<::vocalshaper::SerializableProjectStructure>&, int>(
			"VocalSharp.VocalShaper.ClipBoard", "PasteItem") ||
		!jmadf::CheckInterface<juce::StringArray&>(
			"VocalSharp.VocalShaper.ClipBoard", "GetList") ||
		!jmadf::CheckInterface<int&>(
			"VocalSharp.VocalShaper.ClipBoard", "GetSize") ||
		!jmadf::CheckInterface<void>(
			"VocalSharp.VocalShaper.ClipBoard", "Clean")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.ClipBoard:Bad Interfaces!");
		return false;
	}

	this->editorComp = std::make_unique<EditorComponent>();
	if (!this->editorComp) {
		jmadf::RaiseException("Can't alloc memory space for editor!");
		return false;
	}

	auto ptrEditor = this->editorComp.get();
	jmadf::RegisterInterface<juce::Component*&>(
		"GetPtr",
		[ptrEditor](const juce::String& caller, juce::Component*& ptr) {
			ptr = ptrEditor;
		}
	);

	return true;
}

void VocalSharp_VocalShaper_MainEditor::destory()
{
	this->editorComp = nullptr;
	jmadf::CallInterface<void>(
		"WuChang.JMADF.GlobalConfigs", "Close");
	jmadf::CallInterface<void>(
		"WuChang.JMADF.Translates", "Close");
	jmadf::CallInterface<void>(
		"WuChang.JMADF.LookAndFeelConfigs", "Close");
	jmadf::CallInterface<void>(
		"WuChang.JMADF.DynamicRC", "Unload");
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.CommandManager", "Close");
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ProjectHub", "Release");
}