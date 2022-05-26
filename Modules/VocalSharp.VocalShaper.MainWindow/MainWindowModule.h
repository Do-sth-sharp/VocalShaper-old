#pragma once
#include <libJModule.h>
#include "MainWindow.h"

class MainWindowModule final :
	public jmadf::Module
{
public:
	MainWindowModule();
	~MainWindowModule();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.MainWindow"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<MainWindow> mainWindow;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindowModule)
};
