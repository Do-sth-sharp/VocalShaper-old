#pragma once
#include <JuceHeader.h>

class StretchableBarLAF final : public juce::LookAndFeel_V4
{
public:
	StretchableBarLAF(juce::Colour mainColour) 
		:LookAndFeel_V4(), mainColour(mainColour) {};
	~StretchableBarLAF() override = default;

	void drawStretchableLayoutResizerBar(juce::Graphics& g, int /*w*/, int /*h*/, bool /*isVerticalBar*/,
		bool /*isMouseOver*/, bool isMouseDragging) override
	{
		g.fillAll(this->mainColour);
		if (isMouseDragging)
			g.fillAll(this->mainColour.contrasting(0.2f));
	}

private:
	juce::Colour mainColour;
};