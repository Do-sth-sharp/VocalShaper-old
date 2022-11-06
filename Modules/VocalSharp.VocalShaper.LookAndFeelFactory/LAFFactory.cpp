#include "LAFFactory.h"
#include "StartMenuButtonLAF.h"
#include "PlayBarTextButtonLAF.h"
#include "StretchableBarLAF.h"
#include "StatusBarRoundButttonLAF.h"
#include "StatusBarFlatTextButtonLAF.h"
#include "LabelEditorCallOutBoxLAF.h"

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

juce::LookAndFeel* LAFFactory::getLabelEditorComboBoxLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getLabelEditorCodeEditorLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getLabelEditorResultLabelLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getLabelEditorButtonLAF(const juce::String& caller)
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getLabelEditorCallOutBoxLAF(const juce::String& caller,
	const std::function<int()>& borderSizeFunc, const std::function<float()>& cornerSizeFunc,
	const juce::Colour colorBackground, const juce::Colour colorBorder)
{
	auto ptrLAF = new LabelEditorCallOutBoxLAF(borderSizeFunc, cornerSizeFunc, colorBackground, colorBorder);
	this->addToList(caller, ptrLAF);
	return ptrLAF;
}

void LAFFactory::close(const juce::String& caller)
{
	juce::ScopedWriteLock sLock(this->setLock);
	if (this->mSet.find(caller) == this->mSet.end()) {
		return;
	}
	this->mSet.erase(caller);

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
	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.insert(caller);

	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	this->lafs.push_back(std::make_pair(caller, std::move(std::unique_ptr<juce::LookAndFeel>(laf))));
}