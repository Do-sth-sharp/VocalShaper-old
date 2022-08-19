#include "VocalSharp_VocalShaper_MainWindow.h"
#include <libJModule.h>

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

	if (!jmadf::GetException().isEmpty()) {
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
			result = this->mainWindow->newProj(name, path);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
		"CopyProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path,
			const juce::String& nameSrc, const juce::String& pathSrc, bool& result)
		{
			result = this->mainWindow->copyProj(name, path, nameSrc, pathSrc);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"OpenProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = this->mainWindow->openProj(name, path);
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
	
	this->mainWindow = std::make_unique<MainWindow>("VocalShaper");
	if (!this->mainWindow) {
		jmadf::RaiseException("Can't alloc memory space for main window!");
		return false;
	}
	if (openglLoaded) {
		jmadf::CallInterface<juce::Component*>(
			"WuChang.JMADF.OpenGLComponentRender", "Attach",
			this->mainWindow.get()
			);
	}
	return true;
}

void VocalSharp_VocalShaper_MainWindow::destory()
{
	if (jmadf::ModuleIsLoaded("WuChang.JMADF.OpenGLComponentRender")) {
		jmadf::CallInterface<void>(
			"WuChang.JMADF.OpenGLComponentRender", "Detach"
			);
	}
	this->mainWindow = nullptr;
	jmadf::CallInterface<void>(
		"WuChang.JMADF.LookAndFeelConfigs", "Close"
		);
	jmadf::CallInterface<void>(
		"WuChang.JMADF.DynamicRC",
		"Unload"
		);
}