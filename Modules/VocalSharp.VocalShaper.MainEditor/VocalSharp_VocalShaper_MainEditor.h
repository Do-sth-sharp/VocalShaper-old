#pragma once
#include <libJModule.h>
#include "EditorComponent.h"

class VocalSharp_VocalShaper_MainEditor final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_MainEditor();
	~VocalSharp_VocalShaper_MainEditor();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.MainEditor"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
private:
	std::unique_ptr<EditorComponent> editorComp;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_MainEditor)
};
