#pragma once
#include <JuceHeader.h>

class SMComponent final : public juce::Component
{
public:
	SMComponent();
	~SMComponent() override;
	
	void paint(juce::Graphics& g) override;
	
private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour leftBar;
	}colors;
	struct Sizes final
	{
		double width_leftBar;
		double width_leftBar_max;
	}sizes;
	struct Positions final
	{
		double posY_logo;
	}positions;
	struct Scales final
	{
		double width_logo;
	}scales;
	
	juce::Image logoImage;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SMComponent)
};