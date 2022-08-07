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
		juce::Colour text_title;
	}colors;//界面颜色
	struct Sizes final
	{
		double height_font;
		double width_leftBar;
		double width_leftBar_max;
		double height_logo_topMargin;
		double width_button;
		double height_button;
		double height_button_logo_split;
		double height_button_split;
		double height_titleBar;
		double height_titleBar_max;
		double width_title_leftMargin;
	}sizes;//控件大小
	struct Positions final
	{
	}positions;//控件位置
	struct Scales final
	{
		double width_logo;
		double height_title;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* button;
	}lafs;//控件样式
	
	juce::Image logoImage;
	std::unique_ptr<juce::TextButton> btNewProj, btOpenProj;

	std::function<const juce::String(const juce::String&)> tr;
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SMComponent)
};