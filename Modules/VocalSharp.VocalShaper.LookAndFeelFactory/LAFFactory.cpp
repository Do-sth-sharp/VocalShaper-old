#include "LAFFactory.h"
#include "StartMenuButtonLAF.h"
#include "PlayBarTextButtonLAF.h"
#include "StretchableBarLAF.h"
#include "StatusBarRoundButttonLAF.h"
#include "StatusBarFlatTextButtonLAF.h"

juce::LookAndFeel* LAFFactory::getStartMenuButtonLAF(const juce::String& caller, const std::function<int()>& fontHeightFunc)
{
	auto ptrLAF = new StartMenuButtonLAF(fontHeightFunc);
	this->addToList(caller, ptrLAF);
	this->addToFTemp(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStartMenuTextEditorLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStartMenuListBoxLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getMainMenuButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getTabCloseButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getPlayButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getPlayTextButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new PlayBarTextButtonLAF;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getToolButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStretchableBarLAF(const juce::String& caller, juce::Colour mainColour)
{
	auto ptrLAF = new StretchableBarLAF(mainColour);
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStatusButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStatusTextButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStatusRoundButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new StatusBarRoundButtonLAF;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStatusFlatTextButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new StatusBarFlatTextButtonLAF;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

void LAFFactory::close(const juce::String& caller)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	this->lafs.remove_if([caller](const std::pair<juce::String, std::unique_ptr<juce::LookAndFeel>>& pair)
		{return pair.first == caller; });
	this->funcLTemp.remove_if([caller](const std::pair<juce::String, StartMenuButtonLAF*>& pair)
		{return pair.first == caller; });
}

void LAFFactory::releaseFunc(const juce::String& caller)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	for (auto& p : this->funcLTemp) {
		if (p.first == caller) {
			p.second->fontHeightFunc = [] {return 0; };
		}
	}
}

void LAFFactory::addToList(const juce::String& caller, juce::LookAndFeel* laf)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	this->lafs.push_back(std::make_pair(caller, std::move(std::unique_ptr<juce::LookAndFeel>(laf))));
}

void LAFFactory::addToFTemp(const juce::String& caller, StartMenuButtonLAF* laf)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	this->funcLTemp.push_back(std::make_pair(caller, laf));
}