#pragma once
#include <JuceHeader.h>

class StartMenuButtonLAF final : public juce::LookAndFeel_V4
{
public:
	StartMenuButtonLAF(const std::function<int()>& fontHeightFunc) :LookAndFeel_V4(), fontHeightFunc(fontHeightFunc) {};
	~StartMenuButtonLAF() override = default;
	juce::Font getTextButtonFont(juce::TextButton&, int) override
	{
		return { (float)this->fontHeightFunc() };
	};
private:
	const std::function<int()> fontHeightFunc;
};