#include "DRCModule.h"

DRCModule::DRCModule()
	:Module()
{
	
}

DRCModule::~DRCModule()
{
	
}

bool DRCModule::init()
{
	this->rcManager = std::make_unique<RCPool>();
	if (!this->rcManager) {
		return false;
	}
	
	RCPool* ptrRCP = this->rcManager.get();
	jmadf::RegisterInterface<const juce::String&, std::pair<size_t&, void*&>>(
		"GetRC",
		[ptrRCP](const juce::String& caller, const juce::String& path, std::pair<size_t&, void*&>mem) {
			mem = ptrRCP->get(caller, path);
		}
	);
	jmadf::RegisterInterface<const juce::String&, std::pair<size_t&, void*&>>(
		"CopyRC",
		[ptrRCP](const juce::String& caller, const juce::String& path, std::pair<size_t&, void*&>mem) {
			mem = ptrRCP->copy(caller, path);
		}
	);
	jmadf::RegisterInterface<const juce::String&>(
		"ReleaseRC",
		[ptrRCP](const juce::String& caller, const juce::String& path) {
			ptrRCP->release(caller, path);
		}
	);
	jmadf::RegisterInterface<void>(
		"Unload",
		[ptrRCP](const juce::String& caller) {
			ptrRCP->destory(caller);
		}
	);
	
	return true;
}

void DRCModule::destory()
{
}