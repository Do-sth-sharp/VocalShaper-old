#include "CallbackReactor.h"

void CallbackReactor::addSaveCallback(const juce::String& caller, const vocalshaper::ProjectProxy::SaveCallbackFunc& func)
{
	juce::ScopedWriteLock locker(this->saveLock);
	this->saveCallbacks.add(std::make_pair(caller, func));
}

void CallbackReactor::addCloseCallback(const juce::String& caller, const vocalshaper::ProjectProxy::CloseCallbackFunc& func)
{
	juce::ScopedWriteLock locker(this->closeLock);
	this->closeCallbacks.add(std::make_pair(caller, func));
}

void CallbackReactor::addEventHandler(const juce::String& caller, const vocalshaper::EventProcesser::EventHandleFunc& func)
{
	juce::ScopedWriteLock locker(this->eventHandlerLock);
	this->eventHandlers.add(std::make_pair(caller, func));
}

void CallbackReactor::addActionRules(const juce::String& caller, const vocalshaper::actions::ActionBase::RuleFunc& func)
{
	juce::ScopedWriteLock locker(this->rulesLock);
	this->actionRules.add(std::make_pair(caller, func));
}

void CallbackReactor::release(const juce::String& caller)
{
	{
		juce::ScopedWriteLock locker(this->saveLock);
		this->saveCallbacks.removeIf([&caller](const std::pair<juce::String, vocalshaper::ProjectProxy::SaveCallbackFunc>& value)
			{return value.first == caller; });
	}
	{
		juce::ScopedWriteLock locker(this->closeLock);
		this->closeCallbacks.removeIf([&caller](const std::pair<juce::String, vocalshaper::ProjectProxy::CloseCallbackFunc>& value)
			{return value.first == caller; });
	}
	{
		juce::ScopedWriteLock locker(this->eventHandlerLock);
		this->eventHandlers.removeIf([&caller](const std::pair<juce::String, vocalshaper::EventProcesser::EventHandleFunc>& value)
			{return value.first == caller; });
	}
	{
		juce::ScopedWriteLock locker(this->rulesLock);
		this->actionRules.removeIf([&caller](const std::pair<juce::String, vocalshaper::actions::ActionBase::RuleFunc>& value)
			{return value.first == caller; });
	}
}

void CallbackReactor::process(vocalshaper::ProjectProxy* proj)
{
	if (!proj) {
		return;
	}
	juce::ScopedWriteLock locker(proj->getLock());

	proj->addSaveCallbacks({ [this](const vocalshaper::ProjectProxy* proj) {this->callSaveCallbacks(proj); } });
	proj->addCloseCallbacks({ [this](const vocalshaper::ProjectProxy* proj) {this->callCloseCallbacks(proj); } });
	auto events = proj->getProcesser();
	if (events) {
		events->addEventHandles({ [this](vocalshaper::actions::ActionBase& action) {this->callEventHandlers(action); } });
		events->addEventRules({ [this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
			{this->callActionRules(action,type); } });
	}
}

void CallbackReactor::callSaveCallbacks(const vocalshaper::ProjectProxy* proj)
{
	juce::ScopedReadLock locker(this->saveLock);
	for (auto& f : this->saveCallbacks) {
		f.second(proj);
	}
}

void CallbackReactor::callCloseCallbacks(const vocalshaper::ProjectProxy* proj)
{
	juce::ScopedReadLock locker(this->closeLock);
	for (auto& f : this->closeCallbacks) {
		f.second(proj);
	}
}

void CallbackReactor::callEventHandlers(vocalshaper::actions::ActionBase& action)
{
	juce::ScopedReadLock locker(this->eventHandlerLock);
	for (auto& f : this->eventHandlers) {
		f.second(action);
	}
}

void CallbackReactor::callActionRules(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock locker(this->rulesLock);
	for (auto& f : this->actionRules) {
		f.second(action, type);
	}
}
