#pragma once
#include <JuceHeader.h>

class TopEditor final : public juce::Component
{
public:
	TopEditor();
	~TopEditor() override = default;

	void paint(juce::Graphics& g) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopEditor);
};