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
		jmadf::LoadModule("VocalSharp.VocalShaper.CommandManager") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.ClipBoard") && 
		jmadf::LoadModule("VocalSharp.VocalShaper.TrackEditor") &&
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
		!jmadf::CheckInterface<juce::LookAndFeel*&, juce::Colour>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStretchableBarLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetToolButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStatusButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStatusTextButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStatusRoundButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStatusFlatTextButtonLAF")
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
			"VocalSharp.VocalShaper.ProjectHub", "AddNotice")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.ProjectHub:Bad Interfaces!");
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
	if (
		!jmadf::CheckInterface<vocalshaper::EditorBase*&>(
			"VocalSharp.VocalShaper.TrackEditor", "GetPtr")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.TrackEditor:Bad Interfaces!");
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

	this->editorComp = std::make_unique<EditorComponent>();
	if (!this->editorComp) {
		jmadf::RaiseException("Can't alloc memory space for editor!");
		return false;
	}

	auto ptrEditor = this->editorComp.get();
	jmadf::RegisterInterface<juce::Component*&>(
		"GetPtr",
		[ptrEditor](const juce::String&, juce::Component*& ptr) {
			ptr = ptrEditor;
		}
	);

	jmadf::RegisterInterface<vocalshaper::AdsorbState>(
		"SetAdsorb",
		[ptrEditor](const juce::String&, vocalshaper::AdsorbState state) {
			ptrEditor->setAdsorb(state);
		}
	);
	jmadf::RegisterInterface<vocalshaper::GridState>(
		"SetGrid",
		[ptrEditor](const juce::String&, vocalshaper::GridState state) {
			ptrEditor->setGrid(state);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_MainEditor::destory()
{
	this->editorComp = nullptr;
}