#pragma once
#include <JuceHeader.h>

class StatusBar final : public juce::Component
{
public:
	StatusBar();
	~StatusBar() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Colors final
	{
		juce::Colour background_statusBar;
		juce::Colour border_statusBar;

		juce::Colour text_statusButton;
		juce::Colour background_statusButton;
	}colors;//界面颜色
	struct Size final
	{
		double width_statusBorderRight;

		double width_statusMarginRight;
		double width_statusButtonSplit;
		double width_statusButtonGroupSplit;

		double width_statusSplitLine;

		double width_statusButtonMargin;
	}sizes;//控件大小
	struct Scales final
	{
		double height_statusButton;
		double height_statusSplitLine;
	}scales;//控件缩放
	struct LookAndFeels final
	{
	}lafs;//控件样式

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBar)
};