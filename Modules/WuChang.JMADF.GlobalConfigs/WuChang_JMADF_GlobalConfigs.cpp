#include "WuChang_JMADF_GlobalConfigs.h"

bool WuChang_JMADF_GlobalConfigs::init()
{
	this->configs = std::make_unique<GlobalConfigs>();
	
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
	jmadf::RegisterInterface<void>(
		"Close",
		[this](const juce::String& caller)
		{
			this->configs->close(caller);
		}
	);
	
	return true;
}

void WuChang_JMADF_GlobalConfigs::destory()
{
	this->configs = nullptr;
}