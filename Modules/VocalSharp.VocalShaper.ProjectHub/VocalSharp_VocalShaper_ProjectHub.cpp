#include "VocalSharp_VocalShaper_ProjectHub.h"
#include <libJModule.h>

VocalSharp_VocalShaper_ProjectHub::VocalSharp_VocalShaper_ProjectHub()
	:Module()
{
	
}

VocalSharp_VocalShaper_ProjectHub::~VocalSharp_VocalShaper_ProjectHub()
{
	
}

bool VocalSharp_VocalShaper_ProjectHub::init()
{
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"NewProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = this->projects->newProj(name, path);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
		"CopyProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path,
			const juce::String& nameSrc, const juce::String& pathSrc, bool& result)
		{
			result = this->projects->copyProj(name, path, nameSrc, pathSrc);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"OpenProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = this->projects->openProj(name, path);
		}
	);
	return true;
}

void VocalSharp_VocalShaper_ProjectHub::destory()
{
}