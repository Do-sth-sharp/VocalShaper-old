﻿#include "VocalSharp_VocalShaper_MainWindow.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_MainWindow::VocalSharp_VocalShaper_MainWindow()
	:Module()
{
	
}

VocalSharp_VocalShaper_MainWindow::~VocalSharp_VocalShaper_MainWindow()
{
	
}

bool VocalSharp_VocalShaper_MainWindow::init()
{
	jmadf::LoadModule("WuChang.JMADF.LookAndFeelConfigs");
	jmadf::LoadModule("WuChang.JMADF.DynamicRC");
	jmadf::LoadModule("WuChang.JMADF.Device");
	jmadf::LoadModule("VocalSharp.VocalShaper.MainUI");
	jmadf::LoadModule("VocalSharp.VocalShaper.StartMenu");
	jmadf::LoadModule("VocalSharp.VocalShaper.CommandManager");
	jmadf::LoadModule("VocalSharp.VocalShaper.ProjectHub");
	jmadf::LoadModule("WuChang.JMADF.GlobalConfigs");
	jmadf::LoadModule("VocalSharp.VocalShaper.CallbackReactor");
	jmadf::LoadModule("WuChang.JMADF.Translates");

	if (!jmadf::GetException().isEmpty()) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
			"WuChang.JMADF.LookAndFeelConfigs", "GetColor")
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
			"WuChang.JMADF.Device", "GetScreenSize") ||
		!jmadf::CheckInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSizeTruth")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Device:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<bool&>(
			"VocalSharp.VocalShaper.MainUI", "CloseIsAvailable") ||
		!jmadf::CheckInterface<juce::Component*&>(
			"VocalSharp.VocalShaper.MainUI", "GetPtr") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.MainUI", "NewProject") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.MainUI", "CopyProject") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.MainUI", "OpenProject") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.MainUI", "IsOpenedProj") ||
		!jmadf::CheckInterface<const juce::String&, const juce::String&, bool&>(
			"VocalSharp.VocalShaper.MainUI", "IsSavedProj")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.MainUI:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<juce::Component*&>(
			"VocalSharp.VocalShaper.StartMenu", "GetPtr") ||
		!jmadf::CheckInterface<const juce::String&>(
			"VocalSharp.VocalShaper.StartMenu", "OpenPathFromUrl")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.StartMenu:Bad Interfaces!");
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
	if (
		!jmadf::CheckInterface<const std::function<void(const vocalshaper::ProjectProxy*)>&>(
			"VocalSharp.VocalShaper.ProjectHub", "AddNotice")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.ProjectHub:Bad Interfaces!");
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
		!jmadf::CheckInterface<std::function<const juce::String(const juce::String&)>&>(
			"WuChang.JMADF.Translates", "GetFunc")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Translates:Bad Interfaces!");
		return false;
	}

	bool openglLoaded = jmadf::LoadModule("WuChang.JMADF.OpenGLComponentRender");
	if (!jmadf::GetException().isEmpty()) {
		jmadf::ClearException();
	}

	jmadf::RegisterInterface<juce::DocumentWindow*&>(
		"GetMainWindowPtr",
		[this](const juce::String& caller, juce::DocumentWindow*& ptr)
		{
			ptr = this->mainWindow.get();
		}
	);

	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"NewProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			if (this->mainWindow) {
				result = this->mainWindow->newProj(name, path);
			}
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
		"CopyProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path,
			const juce::String& nameSrc, const juce::String& pathSrc, bool& result)
		{
			if (this->mainWindow) {
				result = this->mainWindow->copyProj(name, path, nameSrc, pathSrc);
			}
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"OpenProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			if (this->mainWindow) {
				result = this->mainWindow->openProj(name, path);
			}
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"IsOpenedProj",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			if (this->mainWindow) {
				result = this->mainWindow->isOpenedProj(name, path);
			}
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"IsSavedProj",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			if (this->mainWindow) {
				result = this->mainWindow->isSavedProj(name, path);
			}
		}
	);

	jmadf::RegisterInterface<void>(
		"ShowStartMenu",
		[this](const juce::String& caller)
		{
			if (this->mainWindow) {
				this->mainWindow->showStartMenu();
			}
		}
	);

	jmadf::RegisterInterface<juce::Component*>(
		"MoveToMainWindow",
		[this](const juce::String&, juce::Component* comp) {
			if (this->mainWindow) {
				this->mainWindow->moveSplashIn(comp);
			}
		}
	);

	jmadf::RegisterInterface<const juce::String&>(
		"OpenStringFromUrl",
		[this](const juce::String&, const juce::String& str)
		{
			if (this->mainWindow) {
				this->mainWindow->openStringFromUrl(str);
			}
		}
	);
	jmadf::RegisterInterface<void>(
		"Show",
		[this](const juce::String&)
		{
			if (this->mainWindow) {
				this->mainWindow->setVisible(true);
				this->mainWindow->toFront(false);
			}
		}
	);
	
	//设置内存限制
	{
		juce::var* config = nullptr;
		bool ok = false;
		jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.GlobalConfigs", "GetReference",
			"config", config, ok
			);
		uint64_t sizeLimit = 0;
		if (ok && (config != nullptr)) {
			if ((*config)["FreeMemoryLimit"].isInt64()) {
				sizeLimit = (int64_t)((*config)["FreeMemoryLimit"]);
			}

			vocalshaper::utils::system::MemLimit::setLimitFreeSize(sizeLimit);
		}

		if (!vocalshaper::utils::system::MemLimit::sizeIsNotReachedLimit()) {
			jmadf::RaiseException("No enough memory to start the application!");
			return false;
		}
	}

	this->mainWindow = std::make_unique<MainWindow>("VocalShaper");
	if (!this->mainWindow) {
		jmadf::RaiseException("Can't alloc memory space for main window!");
		return false;
	}
	if (openglLoaded) {
		if (
			jmadf::CheckInterface<juce::Component*>(
				"WuChang.JMADF.OpenGLComponentRender", "Attach") &&
			jmadf::CheckInterface<void>(
				"WuChang.JMADF.OpenGLComponentRender", "Detach")
			) {
			jmadf::CallInterface<juce::Component*>(
				"WuChang.JMADF.OpenGLComponentRender", "Attach",
				this->mainWindow.get()
				);
		}
	}
	return true;
}

void VocalSharp_VocalShaper_MainWindow::destory()
{
	if (jmadf::ModuleIsLoaded("WuChang.JMADF.OpenGLComponentRender")) {
		jmadf::CallInterface<void>(
			"WuChang.JMADF.OpenGLComponentRender", "Detach");
	}
	this->mainWindow = nullptr;
}