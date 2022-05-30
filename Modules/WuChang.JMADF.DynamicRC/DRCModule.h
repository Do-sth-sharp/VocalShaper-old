#pragma once
#include <libJModule.h>
#include "RCPool.h"

class DRCModule final :
	public jmadf::Module
{
public:
	DRCModule();
	~DRCModule();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "WuChang.JMADF.DynamicRC"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
private:
	std::unique_ptr<RCPool> rcManager;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DRCModule)
};
