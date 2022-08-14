#pragma once
#include <JuceHeader.h>

class TabComponent final : public juce::Component
{
public:
	TabComponent();
	~TabComponent() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour icon_mainMenuButton;
		juce::Colour background_mainMenuButton;
	}colors;//������ɫ
	struct Scales final
	{
		double icon_mainMenuButton;
	}scales;//�ؼ�����
	struct LookAndFeels final
	{
		juce::LookAndFeel* mainMenuButton;
	}lafs;//�ؼ���ʽ

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
		= [](juce::Component*, juce::Rectangle<int>&) {};

	std::unique_ptr<juce::Drawable> iconMainMenu;
	std::unique_ptr<juce::DrawableButton> mainMenuButton;

	int heightTemp = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabComponent)
};