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

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectHub)
};