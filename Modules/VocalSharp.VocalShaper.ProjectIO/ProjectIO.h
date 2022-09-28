#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ProjectIO final
{
public:
	ProjectIO();
	~ProjectIO() = default;

	bool read(vocalshaper::ProjectProxy* project) const;
	bool write(vocalshaper::ProjectProxy* project) const;

private:
	juce::String projectExtension;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectIO)
};