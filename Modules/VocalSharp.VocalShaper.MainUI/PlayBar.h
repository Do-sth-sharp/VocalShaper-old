#pragma once
#include <JuceHeader.h>

class PlayBar final : public juce::Component
{
public:
	PlayBar();
	~PlayBar() override = default;

	void play();
	void stop();
	void begin();
	void end();
	void loop();
	void follow();
	void backOnStop();

	bool isPlaying();
	bool isLoop();
	bool isFollow();
	bool isBackOnStop();

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Colors final
	{
		juce::Colour background_playBar;
		juce::Colour icon_playBarButton;
		juce::Colour background_playBarButton;
		juce::Colour icon_playBarButton_highlight;
		juce::Colour background_playBarButton_highlight;
		juce::Colour text_playBar;
		juce::Colour text_playBar_value;

		juce::Colour split_playBar;
	}colors;//界面颜色
	struct Sizes final
	{
		double width_playMarginLeft;
		double width_playMarginRight;
		double width_playButtonSplit;
		double width_playButtonGroupSplit;
		double width_playTextGroupSplit;
		double width_playTextItemSplit;
		double width_playTextSplit;

		double width_playSplitLine;
	}sizes;//控件大小
	struct Scales final
	{
		double height_playButton;
		double height_playText;
		double height_playSplitLine;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* playButton;
	}lafs;//控件样式

	std::function<const juce::String(const juce::String&)> tr;
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	juce::String textAdsorb, textGrid;

	std::unique_ptr<juce::Drawable> iconPlay, iconPlayHighlight, iconStop,
		iconBegin, iconEnd,
		iconLoop, iconFollow, iconLoopHighlight, iconFollowHighlight,
		iconUndo, iconRedo, iconConfig;
	std::unique_ptr<juce::DrawableButton> playButton, stopButton, beginButton, endButton,
		loopButton, followButton, undoButton, redoButton, configButton;

	int playCommandID = -1, stopCommandID = -1, beginCommandID = -1, endCommandID = -1,
		loopCommandID = -1, followCommandID = -1,
		backOnStopCommandID = -1, 
		undoCommandID = -1, redoCommandID = -1;
	juce::ApplicationCommandManager* commandManager = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayBar)
};