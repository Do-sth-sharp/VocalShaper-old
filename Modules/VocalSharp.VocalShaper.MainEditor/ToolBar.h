#pragma once
#include <JuceHeader.h>

class ToolBar final : public juce::Component
{
public:
	ToolBar();
	~ToolBar() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Colors final
	{
		juce::Colour background_toolBar;

		juce::Colour icon_toolBarButton;
		juce::Colour background_toolBarButton;
		juce::Colour icon_toolBarButton_highlight;
		juce::Colour background_toolBarButton_highlight;

		juce::Colour split_toolBar;
		juce::Colour border_toolBar;
	}colors;//界面颜色
	struct Sizes final
	{
		double height_toolMarginTop;
		double height_toolButtonSplit;
		double height_toolButtonGroupSplit;

		double height_toolSplitLine;
		double height_toolBorderTop;
	}sizes;//控件大小
	struct Scales final
	{
		double width_toolButton;
		double width_toolSplitLine;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* toolButton;
	}lafs;//控件样式

	void initCommandID();
	void initCommandFunction();
	void initCommandFlagHook();

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	std::unique_ptr<juce::Drawable> iconViewMode, iconEditMode, 
		iconViewModeHighlight, iconEditModeHighlight;
	std::unique_ptr<juce::DrawableButton> viewModeButton, editModeButton;

	int viewModeCommandID = -1, editModeCommandID = -1;
	juce::ApplicationCommandManager* commandManager = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolBar)
};