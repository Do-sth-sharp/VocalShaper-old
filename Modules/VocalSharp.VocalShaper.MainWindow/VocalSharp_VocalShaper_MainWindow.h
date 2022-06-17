#pragma once
#include <libJModule.h>
#include "MainWindow.h"

class VocalSharp_VocalShaper_MainWindow final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_MainWindow();
	~VocalSharp_VocalShaper_MainWindow();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.MainWindow"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<MainWindow> mainWindow;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_MainWindow)
};
