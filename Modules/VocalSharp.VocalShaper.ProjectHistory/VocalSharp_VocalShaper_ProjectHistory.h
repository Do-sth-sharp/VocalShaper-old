#pragma once
#include <libJModule.h>
#include "HistoryManager.h"

class VocalSharp_VocalShaper_ProjectHistory final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_ProjectHistory();
	~VocalSharp_VocalShaper_ProjectHistory();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.ProjectHistory"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
	std::unique_ptr<HistoryManager> hm;

private:
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_ProjectHistory)
};
