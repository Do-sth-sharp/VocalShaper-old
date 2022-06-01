#pragma once
#include <libJModule.h>

class LookAndFeelFactoryModule final :
	public jmadf::Module
{
public:
	LookAndFeelFactoryModule() = default;
	~LookAndFeelFactoryModule() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.LookAndFeelFactory"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeelFactoryModule)
};