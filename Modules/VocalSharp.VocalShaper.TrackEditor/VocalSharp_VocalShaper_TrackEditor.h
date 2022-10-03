#pragma once
#include <libJModule.h>

class VocalSharp_VocalShaper_TrackEditor final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_TrackEditor();
	~VocalSharp_VocalShaper_TrackEditor();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.TrackEditor"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_TrackEditor)
};
