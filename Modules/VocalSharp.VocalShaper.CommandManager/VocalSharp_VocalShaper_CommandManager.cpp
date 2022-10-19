#include "VocalSharp_VocalShaper_CommandManager.h"

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
			"WuChang.JMADF.Translates", "GetFunc")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Translates:Bad Interfaces!");
		return false;
	}

	this->commands = std::make_unique<CommandManager>();

	jmadf::RegisterInterface<const juce::String&, const CommandManager::CommandFunction&>(
		"RegisterFunction",
		[this](const juce::String& caller,
			const juce::String& name, const CommandManager::CommandFunction& func) {
				this->commands->addCommandFunc(name, func, caller);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const CommandManager::FlagFunction&>(
		"RegisterFlagHook",
		[this](const juce::String& caller,
			const juce::String& name, const CommandManager::FlagFunction& func) {
				this->commands->addFlagFunc(name, func, caller);
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

	jmadf::SetUnloadHook(
		[this](const juce::String& caller) {
			this->commands->release(caller);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_CommandManager::destory()
{
	this->commands = nullptr;
}