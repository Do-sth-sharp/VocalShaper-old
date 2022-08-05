#pragma once
#include <libJModule.h>
#include "Configs.h"

class WuChang_JMADF_Configs final :
	public jmadf::Module
{
public:
	WuChang_JMADF_Configs() = default;
	~WuChang_JMADF_Configs() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.Configs"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<Configs> configs;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WuChang_JMADF_Configs)
};