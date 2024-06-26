#pragma once
#include <libJModule.h>
#include "RCPool.h"

class WuChang_JMADF_DynamicRC final :
	public jmadf::Module
{
public:
	WuChang_JMADF_DynamicRC();
	~WuChang_JMADF_DynamicRC();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "WuChang.JMADF.DynamicRC"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
private:
	std::unique_ptr<RCPool> rcManager;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WuChang_JMADF_DynamicRC)
};
