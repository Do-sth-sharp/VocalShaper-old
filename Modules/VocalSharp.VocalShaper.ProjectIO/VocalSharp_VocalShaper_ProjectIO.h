#pragma once
#include <libJModule.h>
#include "ProjectIO.h"

class VocalSharp_VocalShaper_ProjectIO final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_ProjectIO();
	~VocalSharp_VocalShaper_ProjectIO();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.ProjectIO"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<ProjectIO> projectIO;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_ProjectIO)
};
