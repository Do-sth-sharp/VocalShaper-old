#pragma once
#include <JuceHeader.h>

class LabelEditorCallOutBoxLAF final : public juce::LookAndFeel_V4
{
public:
	LabelEditorCallOutBoxLAF(const std::function<int()>& borderSizeFunc, const std::function<float()>& cornerSizeFunc,
		const juce::Colour colorBackground, const juce::Colour colorBorder)
		:LookAndFeel_V4(), borderSizeFunc(borderSizeFunc),
		cornerSizeFunc(cornerSizeFunc), colorBackground(colorBackground),
		colorBorder(colorBorder) {};
	~LabelEditorCallOutBoxLAF() override = default;

	void drawCallOutBoxBackground(juce::CallOutBox& box, juce::Graphics& g, const juce::Path& path, juce::Image& cachedImage) override
	{
		if (cachedImage.isNull())
		{
			cachedImage = { juce::Image::ARGB, box.getWidth(), box.getHeight(), true };
			juce::Graphics g2(cachedImage);

			juce::DropShadow(juce::Colours::black.withAlpha(0.7f), 8, { 0, 2 }).drawForPath(g2, path);
		}

		g.setColour(juce::Colours::black);
		g.drawImageAt(cachedImage, 0, 0);

		g.setColour(this->colorBackground);
		g.fillPath(path);

		g.setColour(this->colorBorder);
		g.strokePath(path, juce::PathStrokeType(2.0f));
	};

	int getCallOutBoxBorderSize(const juce::CallOutBox&) override
	{
		return this->borderSizeFunc();
	};

	float getCallOutBoxCornerSize(const juce::CallOutBox&) override
	{
		return this->cornerSizeFunc();
	};

private:
	const std::function<int()> borderSizeFunc;
	const std::function<float()> cornerSizeFunc;
	const juce::Colour colorBackground;
	const juce::Colour colorBorder;
};