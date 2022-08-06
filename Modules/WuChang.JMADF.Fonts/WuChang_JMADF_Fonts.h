#pragma once
#include <libJModule.h>
#include "Fonts.h"

class WuChang_JMADF_Fonts final :
	public jmadf::Module
{
public:
	WuChang_JMADF_Fonts() = default;
	~WuChang_JMADF_Fonts() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.Fonts"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<Fonts> fonts;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WuChang_JMADF_Fonts)
};