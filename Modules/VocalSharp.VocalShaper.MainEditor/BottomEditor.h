#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class BottomEditor final : public juce::Component
{
public:
	BottomEditor();
	~BottomEditor() override = default;

	void projectChanged(const vocalshaper::ProjectProxy* ptr);

	void paint(juce::Graphics& g) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomEditor)
};