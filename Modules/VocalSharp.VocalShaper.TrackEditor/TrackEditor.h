#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackEditor final : public vocalshaper::EditorBase
{
public:
	TrackEditor();
	~TrackEditor() override = default;

	void paint(juce::Graphics& g) override;

public:


private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackEditor)
};