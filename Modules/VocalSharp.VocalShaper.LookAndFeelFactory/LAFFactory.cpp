#include "LAFFactory.h"
#include "StartMenuButtonLAF.h"
#include "PlayBarTextButtonLAF.h"
#include "StretchableBarLAF.h"

juce::LookAndFeel* LAFFactory::getStartMenuButtonLAF(const juce::String& caller, const std::function<int()>& fontHeightFunc)
{
	auto ptrLAF = new StartMenuButtonLAF(fontHeightFunc);
	this->addToList(caller, ptrLAF);
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

void LAFFactory::close(const juce::String& caller)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	for (auto it = this->lafs.begin(); it != this->lafs.end();) {
		if ((*it).first == caller) {
			it = this->lafs.erase(it);
			continue;
		}
		it++;
	}
}

void LAFFactory::addToList(const juce::String& caller, juce::LookAndFeel* laf)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	this->lafs.push_back(std::make_pair(caller, std::move(std::unique_ptr<juce::LookAndFeel>(laf))));
}