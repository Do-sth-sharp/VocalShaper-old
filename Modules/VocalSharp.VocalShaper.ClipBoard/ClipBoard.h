#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ClipBoard final
{
public:
	ClipBoard() = default;
	~ClipBoard() = default;

	using ArrayType = juce::OwnedArray<vocalshaper::SerializableProjectStructure>;
	using StackType = juce::OwnedArray<ArrayType>;

	void copyAndCut(ArrayType&& array);
	ArrayType paste();
	ArrayType paste(int index);
	juce::StringArray getList();
	int size();
	void clean();

private:
	StackType list;
	juce::ReadWriteLock lock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipBoard)
};