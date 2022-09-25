#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ClipBoard final
{
public:
	ClipBoard() = default;
	~ClipBoard() = default;

	using ArrayType = juce::OwnedArray<vocalshaper::SerializableProjectStructure>;
	using StackType = juce::Array<ArrayType>;

	void copyAndCut(ArrayType&& array);
	const ArrayType& paste();
	const ArrayType& paste(int index);
	juce::StringArray getList();
	void clean();

private:
	StackType list;
	juce::ReadWriteLock lock;
	ArrayType empty;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipBoard)
};