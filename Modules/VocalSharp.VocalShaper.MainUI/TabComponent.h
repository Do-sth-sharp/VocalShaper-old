#pragma once
#include <JuceHeader.h>
#include "TabList.h"

class TabComponent final : public juce::Component
{
public:
	TabComponent();
	~TabComponent() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

	bool newProj(const juce::String& name, const juce::String& path);
	bool copyProj(const juce::String& name, const juce::String& path,
		const juce::String& nameSrc, const juce::String& pathSrc);
	bool openProj(const juce::String& name, const juce::String& path);
	bool wannaClose();

	void setCaller(const juce::String& caller);

private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour icon_mainMenuButton;
		juce::Colour background_mainMenuButton;
	}colors;//界面颜色
	struct LookAndFeels final
	{
		juce::LookAndFeel* mainMenuButton;
	}lafs;//控件样式

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc
		= [](juce::Component*, juce::Rectangle<int>&) {};

	std::unique_ptr<juce::Drawable> iconMainMenu;
	std::unique_ptr<juce::DrawableButton> mainMenuButton;
	std::unique_ptr<TabList> tabList;

	int heightTemp = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabComponent)
};