#pragma once
#include <JuceHeader.h>

class StartMenuButtonLAF final : public juce::LookAndFeel_V4
{
public:
	StartMenuButtonLAF(int fontHeight) :LookAndFeel_V4(), fontHeight(fontHeight) {};
	~StartMenuButtonLAF() override = default;
	juce::Font getTextButtonFont(juce::TextButton&, int) override
	{
		return { (float)this->fontHeight };
	};
private:
	int fontHeight;
};