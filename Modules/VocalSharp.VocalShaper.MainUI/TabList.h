#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TabList final : public juce::Component
{
public:
	TabList();
	~TabList() override = default;

	bool newProj(const juce::String& name, const juce::String& path);
	bool copyProj(const juce::String& name, const juce::String& path,
		const juce::String& nameSrc, const juce::String& pathSrc);
	bool openProj(const juce::String& name, const juce::String& path);
	bool wannaClose();

	void resized() override;
	void paint(juce::Graphics& g) override;
	void mouseMove(const juce::MouseEvent& event) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseEnter(const juce::MouseEvent& event) override;
	void mouseExit(const juce::MouseEvent& event) override;

private:
	struct Colors final
	{
		juce::Colour background;

		juce::Colour text_tabList;
		juce::Colour background_tabList;
		juce::Colour text_tabList_highlight;
		juce::Colour background_tabList_highlight;
	}colors;//界面颜色
	struct Sizes final
	{
		double width_tabBorder;
		double width_tabMarginRight;
	}sizes;//控件大小
	struct Scales final
	{
		double icon_mainMenuButton;
		double height_closeButton;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* tabCloseButton;
	}lafs;//控件样式

	std::function<void(int, vocalshaper::Project*&)> getPtrFunc;
	std::function<void(int&)> getCurrentFunc, getSizeFunc;

	std::function<const juce::String(const juce::String&)> tr;
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	bool checkThenClose(int index);
	void refreshCompCache();

	juce::Array<std::pair<int, vocalshaper::Project*>> tabShow;
	juce::Array<vocalshaper::Project*> tabHide;
	int currentIndex = -1, hoverIndex = -1;

	std::unique_ptr<juce::DrawableButton> btCloseCurrent, btCloseHover;
	std::unique_ptr<juce::Label> lbDefaultFont;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabList)
};