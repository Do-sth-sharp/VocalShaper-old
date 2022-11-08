#pragma once
#include <JuceHeader.h>

class TimeValueViewer final : public juce::BubbleComponent
{
public:
	TimeValueViewer() = delete;
	TimeValueViewer(juce::Component* parent);

	void setValue(
		double x, double xInBar, double bFromBar, double time, uint8_t beat, uint8_t base, double tempo);
	void showAt(juce::Point<int> pos);
	void close();

protected:
	void getContentSize(int& width, int& height) override;
	void paintContent(juce::Graphics& g, int width, int height) override;

private:
	struct Colors final
	{
		juce::Colour background_timeViewer;
		juce::Colour text_timeViewer;
		juce::Colour value_timeViewer;
	}colors;//界面颜色
	struct Size final
	{
		double width_timeViewerMargin;
		double height_timeViewerMargin;
		double height_timeViewerLineSplit;
		double height_timeViewerText;
		double width_timeViewerTextSplit;

		double height_timeViewerArrow;
	}sizes;//控件大小

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	std::function<const juce::String(const juce::String&)> tr;

	std::unique_ptr<juce::Label> fontLabel = nullptr;

	juce::Component* parent = nullptr;

	double x = 0., xInBar = 0., bFromBar = 0., time = 0., tempo = 120.;
	uint8_t beat = 4, base = 4;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeValueViewer)
};