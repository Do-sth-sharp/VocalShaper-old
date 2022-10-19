#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class CallbackReactor final
{
public:
	CallbackReactor() = default;
	~CallbackReactor() = default;

private:
	juce::Array<vocalshaper::ProjectProxy::SaveCallbackFunc> saveCallbacks;
	juce::Array<vocalshaper::ProjectProxy::CloseCallbackFunc> closeCallbacks;
	juce::Array<vocalshaper::EventProcesser::EventHandleFunc> eventHandlers;
	juce::Array<vocalshaper::actions::ActionBase::RuleFunc> actionRules;
	juce::Array<vocalshaper::actions::ActionBase::CallBackFunc> actionCallbacks;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CallbackReactor)
};