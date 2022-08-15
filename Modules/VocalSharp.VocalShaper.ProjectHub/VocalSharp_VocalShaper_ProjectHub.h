#pragma once
#include <libJModule.h>
#include "ProjectHub.h"

class VocalSharp_VocalShaper_ProjectHub final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_ProjectHub();
	~VocalSharp_VocalShaper_ProjectHub();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.ProjectHub"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<ProjectHub> projects;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_ProjectHub)
};
