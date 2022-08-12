#pragma once
#include <libJModule.h>
#include "UIComponent.h"

class VocalSharp_VocalShaper_MainUI final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_MainUI();
	~VocalSharp_VocalShaper_MainUI();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.MainUI"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
private:
	std::unique_ptr<UIComponent> UIComp;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_MainUI)
};
