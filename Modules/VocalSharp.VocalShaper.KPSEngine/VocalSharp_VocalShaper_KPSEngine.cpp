#include "VocalSharp_VocalShaper_KPSEngine.h"
#include <libJModule.h>
#include <libVocalShaper.h>
#include "KPSEngine.h"

VocalSharp_VocalShaper_KPSEngine::VocalSharp_VocalShaper_KPSEngine()
	:Module()
{
	
}

VocalSharp_VocalShaper_KPSEngine::~VocalSharp_VocalShaper_KPSEngine()
{
	
}

bool VocalSharp_VocalShaper_KPSEngine::init()
{
	jmadf::RegisterInterface<vocalshaper::EngineBase*&>(
		"CreateInstance",
		[](const juce::String&, vocalshaper::EngineBase*& ptr) {
			ptr = new KPSEngine;
		}
	);
	jmadf::RegisterInterface<vocalshaper::EngineBase*&>(
		"DestoryInstance",
		[](const juce::String&, vocalshaper::EngineBase*& ptr) {
			delete ptr; ptr = nullptr;
		}
	);

	return true;
}

void VocalSharp_VocalShaper_KPSEngine::destory()
{
}