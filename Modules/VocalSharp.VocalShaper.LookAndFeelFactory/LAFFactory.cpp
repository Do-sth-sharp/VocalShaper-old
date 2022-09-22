#include "LAFFactory.h"
#include "StartMenuButtonLAF.h"
#include "PlayBarTextButtonLAF.h"
#include "StretchableBarLAF.h"

juce::LookAndFeel* LAFFactory::getStartMenuButtonLAF(int fontHeight)
{
	auto ptrLAF = new StartMenuButtonLAF(fontHeight);
	this->lafs.add(ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStartMenuTextEditorLAF()
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->lafs.add(ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStartMenuListBoxLAF()
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->lafs.add(ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getMainMenuButtonLAF()
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->lafs.add(ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getTabCloseButtonLAF()
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->lafs.add(ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getPlayButtonLAF()
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->lafs.add(ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getPlayTextButtonLAF()
{
	auto ptrLAF = new PlayBarTextButtonLAF;
	this->lafs.add(ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getToolButtonLAF()
{
	auto ptrLAF = new juce::LookAndFeel_V4;
	this->lafs.add(ptrLAF);
	return ptrLAF;
}

juce::LookAndFeel* LAFFactory::getStretchableBarLAF(juce::Colour mainColour)
{
	auto ptrLAF = new StretchableBarLAF(mainColour);
	this->lafs.add(ptrLAF);
	return ptrLAF;
}
