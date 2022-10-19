#include "WuChang_JMADF_Configs.h"

bool WuChang_JMADF_Configs::init()
{
	this->configs = std::make_unique<Configs>();

	jmadf::RegisterInterface<const juce::String&, juce::var*&, bool&>(
		"GetReference",
		[this](const juce::String& caller, const juce::String& fileName, juce::var*& ptr, bool& result)
		{
			result = this->configs->getReference(caller, fileName, ptr);
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
			this->configs->release(caller);
		}
	);
	return true;
}

void WuChang_JMADF_Configs::destory()
{
	this->configs = nullptr;
}