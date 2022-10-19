﻿#include "VocalSharp_VocalShaper_TrackEditor.h"
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
		!jmadf::CheckInterface<void>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "Close")
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
			"VocalSharp.VocalShaper.CommandManager", "GetCommandManager") ||
		!jmadf::CheckInterface<void>(
			"VocalSharp.VocalShaper.CommandManager", "Close")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.CommandManager:Bad Interfaces!");
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
		"VocalSharp.VocalShaper.LookAndFeelFactory", "Close");
}