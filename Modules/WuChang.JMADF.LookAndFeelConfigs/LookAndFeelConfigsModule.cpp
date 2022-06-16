#include "LookAndFeelConfigsModule.h"

bool LookAndFeelConfigsModule::init()
{
	this->configs = std::make_unique<LAFConfigs>();
	
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"GetNumber",
		[this](const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, double& value, bool& result)
		{
			result = this->configs->getNum(caller, className, widget, attribute, value);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"GetString",
		[this](const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, juce::String& value, bool& result)
		{
			result = this->configs->getString(caller, className, widget, attribute, value);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, bool&, bool&>(
		"GetBoolean",
		[this](const juce::String& caller, const juce::String& className, const juce::String& widget, const juce::String& attribute, bool& value, bool& result)
		{
			result = this->configs->getBool(caller, className, widget, attribute, value);
		}
	);
	
	jmadf::RegisterInterface<void>(
		"ReleaseAll",
		[this](const juce::String&)
		{
			this->configs->releaseAll();
		}
	);
	jmadf::RegisterInterface<const juce::String&>(
		"Release",
		[this](const juce::String&, const juce::String& moduleName)
		{
			this->configs->release(moduleName);
		}
	);
	jmadf::RegisterInterface<void>(
		"Close",
		[this](const juce::String& caller)
		{
			this->configs->close(caller);
		}
	);
	
	return true;
}

void LookAndFeelConfigsModule::destory()
{
	this->configs = nullptr;
}