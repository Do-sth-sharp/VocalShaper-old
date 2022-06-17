#pragma once
#include <libJModule.h>
#include "SMComponent.h"

class VocalSharp_VocalShaper_StartMenu final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_StartMenu();
	~VocalSharp_VocalShaper_StartMenu();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.StartMenu"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
private:
	std::unique_ptr<SMComponent> smComp;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_StartMenu)
};
