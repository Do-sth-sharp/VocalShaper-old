#pragma once
#include <libJModule.h>
#include "CallbackReactor.h"

class VocalSharp_VocalShaper_CallbackReactor final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_CallbackReactor();
	~VocalSharp_VocalShaper_CallbackReactor();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.CallbackReactor"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
	std::unique_ptr<CallbackReactor> callbacks = nullptr;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_CallbackReactor)
};
