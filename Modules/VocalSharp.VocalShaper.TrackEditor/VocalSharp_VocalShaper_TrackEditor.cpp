#include "VocalSharp_VocalShaper_TrackEditor.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_TrackEditor::VocalSharp_VocalShaper_TrackEditor()
	:Module()
{
	
}

VocalSharp_VocalShaper_TrackEditor::~VocalSharp_VocalShaper_TrackEditor()
{
	
}

bool VocalSharp_VocalShaper_TrackEditor::init()
{
	if (!(
		jmadf::LoadModule("WuChang.JMADF.LookAndFeelConfigs") &&
		jmadf::LoadModule("WuChang.JMADF.DynamicRC") &&
		jmadf::LoadModule("WuChang.JMADF.Device") &&
		jmadf::LoadModule("WuChang.JMADF.GlobalConfigs") &&
		jmadf::LoadModule("WuChang.JMADF.Translates") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.LookAndFeelFactory") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.CommandManager") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.CallbackReactor") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.ClipBoard") &&
		jmadf::LoadModule("WuChang.JMADF.Fonts")
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
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorComboBoxLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorCodeEditorLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorResultLabelLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&,
			const std::function<int()>&, const std::function<float()>&,
			const juce::Colour, const juce::Colour>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorCallOutBoxLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&,
			const std::function<int()>&, const std::function<float()>&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorBubbleLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetTrackViewSMButtonLAF") ||
		!jmadf::CheckInterface<juce::LookAndFeel*&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetTrackViewLinkButtonLAF")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.LookAndFeelFactory:Bad Interfaces!");
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
	if (
		!jmadf::CheckInterface<const vocalshaper::EditorBase*>(
			"VocalSharp.VocalShaper.ClipBoard", "AcceptCopyAndDelete") ||
		!jmadf::CheckInterface<const vocalshaper::EditorBase*>(
			"VocalSharp.VocalShaper.ClipBoard", "AcceptPaste") ||
		!jmadf::CheckInterface<const vocalshaper::EditorBase*>(
			"VocalSharp.VocalShaper.ClipBoard", "UnacceptCopyAndDelete") ||
		!jmadf::CheckInterface<const vocalshaper::EditorBase*>(
			"VocalSharp.VocalShaper.ClipBoard", "UnacceptPaste")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.ClipBoard:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, juce::Typeface::Ptr&, bool&>(
			"WuChang.JMADF.Fonts", "GetFont")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Fonts:Bad Interfaces!");
		return false;
	}

	this->editor = std::make_unique<TrackEditor>();

	if (!this->editor) {
		jmadf::RaiseException("Can't alloc memory space for editor!");
		return false;
	}

	vocalshaper::EditorBase* ptrEditor = this->editor.get();
	jmadf::RegisterInterface<vocalshaper::EditorBase*&>(
		"GetPtr",
		[ptrEditor](const juce::String&, vocalshaper::EditorBase*& ptr) {
			ptr = ptrEditor;
		}
	);

	return true;
}

void VocalSharp_VocalShaper_TrackEditor::destory()
{
	this->editor = nullptr;
}