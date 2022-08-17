#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ProjectHub final
{
public:
	ProjectHub() = default;
	~ProjectHub() = default;

	bool newProj(const juce::String& name, const juce::String& path);
	bool copyProj(const juce::String& name, const juce::String& path,
		const juce::String& nameSrc, const juce::String& pathSrc);
	bool openProj(const juce::String& name, const juce::String& path);

	void setCurrent(int index);
	void setCurrentAndToFront(int index);
	void close(int index);
	bool checkForClose(int index);
	vocalshaper::Project* get(int index);
	int getCurrent();
	int getSize();
	void save(int index);

private:
	juce::OwnedArray<vocalshaper::Project> projList;
	int currentIndex = -1;

	juce::SpinLock lock;

	inline vocalshaper::Project* create(const juce::String& name, const juce::String& path) const;
	inline int findOpened(const juce::String& name, const juce::String& path) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectHub)
};