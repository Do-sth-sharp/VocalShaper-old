#pragma once
#include <libJModule.h>
#include "Translates.h"

class WuChang_JMADF_Translates final :
	public jmadf::Module
{
public:
	WuChang_JMADF_Translates() = default;
	~WuChang_JMADF_Translates() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.Translates"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<Translates> translates;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WuChang_JMADF_Translates)
};