#include "WuChang_JMADF_LookAndFeelConfigs.h"

bool WuChang_JMADF_LookAndFeelConfigs::init()
{
	this->configs = std::make_unique<LAFConfigs>();
	
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"GetNumber",
		[this](const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, double& value, bool& result)
		{
			result = this->configs->getNum(caller, className, group, attribute, value);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"GetString",
		[this](const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, juce::String& value, bool& result)
		{
			result = this->configs->getString(caller, className, group, attribute, value);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, bool&, bool&>(
		"GetBoolean",
		[this](const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, bool& value, bool& result)
		{
			result = this->configs->getBool(caller, className, group, attribute, value);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"GetColour",
		[this](const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, juce::Colour& value, bool& result)
		{
			result = this->configs->getColour(caller, className, group, attribute, value);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"GetColor",
		[this](const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, juce::Colour& value, bool& result)
		{
			result = this->configs->getColour(caller, className, group, attribute, value);
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

	jmadf::SetUnloadHook(
		[this](const juce::String& caller)
		{
			this->configs->close(caller);
		}
	);
	
	return true;
}

void WuChang_JMADF_LookAndFeelConfigs::destory()
{
	this->configs = nullptr;
}