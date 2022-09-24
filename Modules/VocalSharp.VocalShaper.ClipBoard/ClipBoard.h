#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ClipBoard final
{
public:
	ClipBoard() = default;
	~ClipBoard() = default;

private:
	std::stack<juce::OwnedArray<vocalshaper::SerializableProjectStructure>> list;
	juce::ReadWriteLock lock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipBoard)
};