#pragma once
#include <libJModule.h>
#include "CommandManager.h"

class VocalSharp_VocalShaper_CommandManager final :
	public jmadf::Module
{
public:
	VocalSharp_VocalShaper_CommandManager();
	~VocalSharp_VocalShaper_CommandManager();
	
	bool init() override;
	void destory() override;
	
	const juce::String getModuleName() override { return "VocalSharp.VocalShaper.CommandManager"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<CommandManager> commands = nullptr;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalSharp_VocalShaper_CommandManager)
};
