#pragma once
#include <libJModule.h>
#include "LAFConfigs.h"

class WuChang_JMADF_LookAndFeelConfigs final :
	public jmadf::Module
{
public:
	WuChang_JMADF_LookAndFeelConfigs() = default;
	~WuChang_JMADF_LookAndFeelConfigs() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.LookAndFeelConfigs"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<LAFConfigs> configs;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WuChang_JMADF_LookAndFeelConfigs)
};