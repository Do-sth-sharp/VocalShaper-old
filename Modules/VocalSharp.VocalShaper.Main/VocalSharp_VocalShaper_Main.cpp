﻿#include "VocalSharp_VocalShaper_Main.h"

VocalSharp_VocalShaper_Main::VocalSharp_VocalShaper_Main()
	:Module()
{
	
}

VocalSharp_VocalShaper_Main::~VocalSharp_VocalShaper_Main()
{
	
}

bool VocalSharp_VocalShaper_Main::init()
{
	if (!(jmadf::LoadModule("WuChang.JMADF.Configs") &&
		jmadf::LoadModule("WuChang.JMADF.GlobalConfigs") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.ProjectHub")
		)) {
		return false;
	}

	jmadf::LoadModule("WuChang.JMADF.OpenGLComponentRender");
	if (!jmadf::GetException().isEmpty()) {
		jmadf::ClearException();
	}

	if (
		!jmadf::CheckInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.GlobalConfigs", "GetReference")
		) {
		jmadf::RaiseException("@WuChang.JMADF.GlobalConfigs:Bad Interfaces!");
		return false;
	}

	//获取设置
	juce::var* config = nullptr;
	bool ok = false;
	jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
		"WuChang.JMADF.GlobalConfigs", "GetReference",
		"config", config, ok
		);
	juce::String lang;
	juce::String font;
	if (ok && (config != nullptr)) {
		if ((*config)["Language"].isString()) {
			lang = (*config)["Language"].toString();
		}
		if ((*config)["Font"].isString()) {
			font = (*config)["Font"].toString();
		}
	}

	//设置语言
	if (!jmadf::LoadModule("WuChang.JMADF.Translates")) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&>(
			"WuChang.JMADF.Translates", "SetCurrentLang") ||
		!jmadf::CheckInterface<const juce::StringArray&>(
			"WuChang.JMADF.Translates", "SetDefaultLang")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Translates:Bad Interfaces!");
		return false;
	}
	jmadf::CallInterface<const juce::StringArray&>(
		"WuChang.JMADF.Translates", "SetDefaultLang",
		{ "zh-cn", "en-us" }
	);
	jmadf::CallInterface<const juce::String&>(
		"WuChang.JMADF.Translates", "SetCurrentLang",
		lang
		);

	//设置字体
	if (!jmadf::LoadModule("WuChang.JMADF.Fonts")) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&>(
			"WuChang.JMADF.Fonts", "SetDefaultFont") ||
		!jmadf::CheckInterface<juce::Typeface::Ptr&, bool&>(
			"WuChang.JMADF.Fonts", "GetDefault")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Fonts:Bad Interfaces!");
		return false;
	}
	jmadf::CallInterface<const juce::String&>(
		"WuChang.JMADF.Fonts", "SetDefaultFont",
		font
		);

	bool fontOK = false;
	juce::Typeface::Ptr ptrFont = nullptr;
	jmadf::CallInterface<juce::Typeface::Ptr&, bool&>(
		"WuChang.JMADF.Fonts", "GetDefault",
		ptrFont, fontOK
		);
	if (fontOK) {
		juce::LookAndFeel::getDefaultLookAndFeel()
			.setDefaultSansSerifTypeface(ptrFont);
	}

	//初始化主题配置
	if (!jmadf::LoadModule("VocalSharp.VocalShaper.ThemeManager")) {
		return false;
	}

	//命令初始化
	if (!jmadf::LoadModule("VocalSharp.VocalShaper.CommandManager")) {
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

	//主界面初始化
	const char* mainWindowName = "VocalSharp.VocalShaper.MainWindow";
	if (!jmadf::LoadModule(mainWindowName)) {
		jmadf::RaiseException("@ERROR " + juce::String(mainWindowName));
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.MainWindow", "NewProject") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.MainWindow", "CopyProject") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.MainWindow", "OpenProject") ||
		!jmadf::CheckInterface<void>(
			"VocalSharp.VocalShaper.MainWindow", "ShowStartMenu") ||
		!jmadf::CheckInterface<juce::Component*>(
			"VocalSharp.VocalShaper.MainWindow", "MoveToMainWindow") ||
		!jmadf::CheckInterface<const juce::String&>(
			"VocalSharp.VocalShaper.MainWindow", "OpenStringFromUrl") ||
		!jmadf::CheckInterface<void>(
			"VocalSharp.VocalShaper.MainWindow", "Show")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.MainWindow:Bad Interfaces!");
		return false;
	}

	jmadf::RegisterInterface<juce::Component*>(
		"MoveToMainWindow",
		[mainWindowName](const juce::String&, juce::Component* comp) {
			jmadf::CallInterface<juce::Component*>(
				mainWindowName, "MoveToMainWindow",
				comp
				);
		}
	);
	jmadf::RegisterInterface<const juce::String&>(
		"ParseCommand",
		[mainWindowName](const juce::String&, const juce::String& command) {
			jmadf::CallInterface<const juce::String&>(
				mainWindowName, "OpenStringFromUrl",
				command
				);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_Main::destory()
{
}