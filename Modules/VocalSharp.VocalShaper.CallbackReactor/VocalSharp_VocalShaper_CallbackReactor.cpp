#include "VocalSharp_VocalShaper_CallbackReactor.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_CallbackReactor::VocalSharp_VocalShaper_CallbackReactor()
	:Module()
{
	
}

VocalSharp_VocalShaper_CallbackReactor::~VocalSharp_VocalShaper_CallbackReactor()
{
	
}

bool VocalSharp_VocalShaper_CallbackReactor::init()
{
	this->callbacks = std::make_unique<CallbackReactor>();

	jmadf::RegisterInterface<const vocalshaper::ProjectProxy::SaveCallbackFunc&>(
		"AddSaveCallback",
		[this](const juce::String& caller, const vocalshaper::ProjectProxy::SaveCallbackFunc& func) {
			this->callbacks->addSaveCallback(caller, func);
		}
	);
	jmadf::RegisterInterface<const vocalshaper::ProjectProxy::CloseCallbackFunc&>(
		"AddCloseCallback",
		[this](const juce::String& caller, const vocalshaper::ProjectProxy::CloseCallbackFunc& func) {
			this->callbacks->addCloseCallback(caller, func);
		}
	);
	jmadf::RegisterInterface<const vocalshaper::EventProcesser::EventHandleFunc&>(
		"AddEventHandler",
		[this](const juce::String& caller, const vocalshaper::EventProcesser::EventHandleFunc& func) {
			this->callbacks->addEventHandler(caller, func);
		}
	);
	jmadf::RegisterInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"AddActionRules",
		[this](const juce::String& caller, const vocalshaper::actions::ActionBase::RuleFunc& func) {
			this->callbacks->addActionRules(caller, func);
		}
	);

	jmadf::RegisterInterface<void>(
		"Release",
		[this](const juce::String& caller) {
			this->callbacks->release(caller);
		}
	);

	jmadf::RegisterInterface<vocalshaper::ProjectProxy*>(
		"Process",
		[this](const juce::String&, vocalshaper::ProjectProxy* proj) {
			this->callbacks->process(proj);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_CallbackReactor::destory()
{
	this->callbacks = nullptr;
}