﻿#include "VocalSharp_VocalShaper_CommandManager.h"

VocalSharp_VocalShaper_CommandManager::VocalSharp_VocalShaper_CommandManager()
	:Module()
{
	
}

VocalSharp_VocalShaper_CommandManager::~VocalSharp_VocalShaper_CommandManager()
{
	
}

bool VocalSharp_VocalShaper_CommandManager::init()
{
	if (!(
		jmadf::LoadModule("WuChang.JMADF.Translates")
		)) {
		return false;
	}
	if (
		!jmadf::CheckInterface<std::function<const juce::String(const juce::String&)>&>(
			"WuChang.JMADF.Translates", "GetFunc") ||
		!jmadf::CheckInterface<void>(
			"WuChang.JMADF.Translates", "Close")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Translates:Bad Interfaces!");
		return false;
	}

	this->commands = std::make_unique<CommandManager>();

	jmadf::RegisterInterface<const juce::String&, CommandManager::CommandFunction>(
		"RegisterFunction",
		[this](const juce::String&,
			const juce::String& name, CommandManager::CommandFunction func) {
				this->commands->addCommandFunc(name, func);
		}
	);
	jmadf::RegisterInterface<const juce::String&, int&>(
		"GetCommandID",
		[this](const juce::String&, const juce::String& name, int& result) {
			result = this->commands->getCommandID(name);
		}
	);
	jmadf::RegisterInterface<juce::ApplicationCommandManager*&>(
		"GetCommandManager",
		[this](const juce::String&, juce::ApplicationCommandManager*& ref) {
			ref = this->commands->getManager();
		}
	);

	return true;
}

void VocalSharp_VocalShaper_CommandManager::destory()
{
	this->commands = nullptr;
	jmadf::CallInterface<void>(
		"WuChang.JMADF.Translates", "Close"
		);
}