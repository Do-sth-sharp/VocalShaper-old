#pragma once
#include <libJModule.h>
#include "ClipBoard.h"

class VocalSharp_VocalShaper_ClipBoard final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_ClipBoard();
	~VocalSharp_VocalShaper_ClipBoard();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.ClipBoard"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
	std::unique_ptr<ClipBoard> clipBoard;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_ClipBoard)
};
