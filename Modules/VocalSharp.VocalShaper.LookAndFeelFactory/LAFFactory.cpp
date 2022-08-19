#include "LAFFactory.h"
#include "StartMenuButtonLAF.h"

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
