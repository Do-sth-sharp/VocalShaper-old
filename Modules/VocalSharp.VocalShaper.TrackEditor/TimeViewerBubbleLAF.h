#pragma once
#include <JuceHeader.h>

class TimeViewerBubbleLAF final : public juce::LookAndFeel_V4
{
public:
	TimeViewerBubbleLAF(const std::function<int()>& borderSizeFunc, const std::function<float()>& cornerSizeFunc)
		:LookAndFeel_V4(), borderSizeFunc(borderSizeFunc),
		cornerSizeFunc(cornerSizeFunc) {};
	~TimeViewerBubbleLAF() override = default;

	void drawBubble(juce::Graphics& g, juce::BubbleComponent& comp,
		const juce::Point<float>& tip,
		const juce::Rectangle<float>& body) override
	{
		juce::Path p;
		p.addBubble(body.reduced(0.5f), body.getUnion(juce::Rectangle<float>(tip.x, tip.y, 1.0f, 1.0f)),
			tip, this->cornerSizeFunc(), juce::jmin(15.0f, body.getWidth() * 0.2f, body.getHeight() * 0.2f));

		g.setColour(comp.findColour(juce::BubbleComponent::backgroundColourId));
		g.fillPath(p);

		g.setColour(comp.findColour(juce::BubbleComponent::outlineColourId));
		g.strokePath(p, juce::PathStrokeType(this->borderSizeFunc()));
	};

private:
	const std::function<int()> borderSizeFunc;
	const std::function<float()> cornerSizeFunc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeViewerBubbleLAF)
};