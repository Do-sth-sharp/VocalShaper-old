#pragma once
#include <JuceHeader.h>

class HistoryManager final
{
public:
	HistoryManager();
	~HistoryManager();

	int getSize();
	const juce::String getName(int index);
	const juce::String getPath(int index);
	const juce::String getTime(int index);
	void add(const juce::String& name, const juce::String& path);
	void open(int index);
	void remove(int index);

	void setFilter(const juce::String& filter);

private:
	int maxCount = 0;
	const juce::String path = "/.data/history.json";

	juce::Array<juce::var> data;
	juce::Array<bool> hide;

	juce::String filterStr;

	juce::ReadWriteLock lock;

	void save();
	void cut();
	bool match(juce::var& object);
	void filter();
	const juce::String createTime();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HistoryManager)
};