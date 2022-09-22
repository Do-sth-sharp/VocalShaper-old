#pragma once
#include <JuceHeader.h>

class BottomEditor final : public juce::Component
{
public:
	BottomEditor();
	~BottomEditor() override = default;

	void paint(juce::Graphics& g) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomEditor)
};