#pragma once
#include <libJModule.h>

class VocalSharp_VocalShaper_ThemeManager final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_ThemeManager() = default;
	~VocalSharp_VocalShaper_ThemeManager() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.ThemeManager"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_ThemeManager)
};