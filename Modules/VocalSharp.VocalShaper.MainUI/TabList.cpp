#include "TabList.h"

TabList::TabList()
	:Component("Tab List Component")
{

}

bool TabList::newProj(const juce::String& name, const juce::String& path)
{
	return true;
}

bool TabList::copyProj(const juce::String& name, const juce::String& path,
	const juce::String& nameSrc, const juce::String& pathSrc)
{
	return true;
}

bool TabList::openProj(const juce::String& name, const juce::String& path)
{
	return true;
}

bool TabList::wannaClose()
{
	return true;
}
