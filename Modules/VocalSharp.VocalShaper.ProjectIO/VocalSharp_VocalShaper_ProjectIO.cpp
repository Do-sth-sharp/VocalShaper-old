#include "VocalSharp_VocalShaper_ProjectIO.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_ProjectIO::VocalSharp_VocalShaper_ProjectIO()
	:Module()
{
	
}

VocalSharp_VocalShaper_ProjectIO::~VocalSharp_VocalShaper_ProjectIO()
{
	
}

bool VocalSharp_VocalShaper_ProjectIO::init()
{
	if (!jmadf::LoadModule("WuChang.JMADF.GlobalConfigs")) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.GlobalConfigs", "GetReference") ||
		!jmadf::CheckInterface<void>(
			"WuChang.JMADF.GlobalConfigs", "Close")
		) {
		jmadf::RaiseException("@WuChang.JMADF.GlobalConfigs:Bad Interfaces!");
		return false;
	}

	this->projectIO = std::make_unique<ProjectIO>();

	jmadf::RegisterInterface<vocalshaper::ProjectProxy*, bool&>(
		"ReadProject",
		[this](const juce::String&,
			vocalshaper::ProjectProxy* project, bool& result)
		{
			result = this->projectIO->read(project);
		}
	);
	jmadf::RegisterInterface<vocalshaper::ProjectProxy*, bool&>(
		"WriteProject",
		[this](const juce::String&,
			vocalshaper::ProjectProxy* project, bool& result)
		{
			result = this->projectIO->write(project);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_ProjectIO::destory()
{
	this->projectIO = nullptr;
}