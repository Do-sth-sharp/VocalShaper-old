#pragma once
#include <libJModule.h>

class LookAndFeelConfigsModule final :
	public jmadf::Module
{
public:
	LookAndFeelConfigsModule() = default;
	~LookAndFeelConfigsModule() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.LookAndFeelConfigs"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeelConfigsModule)
};