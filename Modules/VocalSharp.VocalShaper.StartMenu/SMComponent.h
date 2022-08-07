#pragma once
#include <JuceHeader.h>

class SMComponent final : public juce::Component
{
public:
	SMComponent();
	~SMComponent() override;
	
	void paint(juce::Graphics& g) override;
	void resized() override;
	
private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour leftBar;
		juce::Colour text_button;
		juce::Colour background_button;
	}colors;
	struct Sizes final
	{
		double width_leftBar;
		double width_leftBar_max;
		double width_button;
		double height_button;
		double height_button_logo_split;
		double height_button_split;
	}sizes;
	struct Positions final
	{
		double posY_logo;
	}positions;
	struct Scales final
	{
		double width_logo;
	}scales;
	struct LookAndFeels final
	{
		std::unique_ptr<juce::LookAndFeel_V4> button;
	}lafs;
	
	juce::Image logoImage;
	std::unique_ptr<juce::TextButton> btNewProj, btOpenProj;

	std::function<const juce::String(const juce::String&)> tr;
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SMComponent)
};