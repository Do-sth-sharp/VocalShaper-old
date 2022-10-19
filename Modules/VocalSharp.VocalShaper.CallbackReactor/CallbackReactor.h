#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class CallbackReactor final
{
public:
	CallbackReactor() = default;
	~CallbackReactor() = default;

public:
	void addSaveCallback(const juce::String& caller, const vocalshaper::ProjectProxy::SaveCallbackFunc& func);
	void addCloseCallback(const juce::String& caller, const vocalshaper::ProjectProxy::CloseCallbackFunc& func);
	void addEventHandler(const juce::String& caller, const vocalshaper::EventProcesser::EventHandleFunc& func);
	void addActionRules(const juce::String& caller, const vocalshaper::actions::ActionBase::RuleFunc& func);

	void release(const juce::String& caller);

public:
	void process(vocalshaper::ProjectProxy* proj);

public:
	void callSaveCallbacks(const vocalshaper::ProjectProxy* proj);
	void callCloseCallbacks(const vocalshaper::ProjectProxy* proj);
	void callEventHandlers(vocalshaper::actions::ActionBase& action);
	void callActionRules(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);

private:
	juce::Array<std::pair<juce::String, vocalshaper::ProjectProxy::SaveCallbackFunc>> saveCallbacks;
	juce::Array<std::pair<juce::String, vocalshaper::ProjectProxy::CloseCallbackFunc>> closeCallbacks;
	juce::Array<std::pair<juce::String, vocalshaper::EventProcesser::EventHandleFunc>> eventHandlers;
	juce::Array<std::pair<juce::String, vocalshaper::actions::ActionBase::RuleFunc>> actionRules;

	juce::ReadWriteLock saveLock, closeLock, eventHandlerLock, rulesLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CallbackReactor)
};