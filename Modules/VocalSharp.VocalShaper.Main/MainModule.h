#pragma once
#include "libJModule/libJModule.h"

class MainModule :
    public JModuleBase
{
public:
	MainModule();
	~MainModule();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.Main"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainModule)
};
