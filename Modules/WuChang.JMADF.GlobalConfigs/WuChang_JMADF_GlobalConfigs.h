#pragma once
#include <libJModule.h>
#include "GlobalConfigs.h"

class WuChang_JMADF_GlobalConfigs final :
	public jmadf::Module
{
public:
	WuChang_JMADF_GlobalConfigs() = default;
	~WuChang_JMADF_GlobalConfigs() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.GlobalConfigs"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<GlobalConfigs> configs;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WuChang_JMADF_GlobalConfigs)
};