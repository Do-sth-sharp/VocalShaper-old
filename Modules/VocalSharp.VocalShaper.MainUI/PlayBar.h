#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

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

	void projectChanged(const vocalshaper::ProjectProxy* ptr);

	void resized() override;
	void paint(juce::Graphics& g) override;

	enum class AdsorbState
	{
		Adsorb1Beat = 1,
		Adsorb1_2Beat = 2,
		Adsorb1_4Beat = 4,
		Adsorb1_6Beat = 6,
		Adsorb1_8Beat = 8,
		Adsorb1_12Beat = 12,
		Adsorb1_16Beat = 16,
		Adsorb1_24Beat = 24,
		Adsorb1_32Beat = 32,
		AdsorbOff = 0
	};

	enum class GridState
	{
		Grid1Beat = 1,
		Grid1_2Beat = 2,
		Grid1_4Beat = 4,
		Grid1_6Beat = 6,
		Grid1_8Beat = 8,
		Grid1_12Beat = 12,
		Grid1_16Beat = 16,
		Grid1_24Beat = 24,
		Grid1_32Beat = 32
	};

	AdsorbState getAdsorb();
	void setAdsorb(AdsorbState state);
	GridState getGrid();
	void setGrid(GridState state);

	void showAdsorbMenu();
	void showGridMenu();

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
		double width_playTextButtonMargin;
		double width_playTextGroupSplit;
		double width_playTextItemSplit;
		double width_playTextSplit;

		double width_playSplitLine;
	}sizes;//控件大小
	struct Scales final
	{
		double height_playButton;
		double height_playTextButton;
		double height_playSplitLine;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* playButton;
		juce::LookAndFeel* playTextButton;
	}lafs;//控件样式

	void initCommandID();
	void initCommandFunction();
	void initCommandFlagHook();

	void refreshAdsorbButton();
	void refreshGridButton();

	juce::PopupMenu createAdsorbMenu();
	juce::PopupMenu createGridMenu();

	std::function<const juce::String(const juce::String&)> tr;
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	juce::String textAdsorb, textGrid;

	AdsorbState adsorbState = AdsorbState::Adsorb1Beat;
	GridState gridState = GridState::Grid1Beat;

	std::unique_ptr<juce::Drawable> iconPlay, iconPlayHighlight, iconStop,
		iconBegin, iconEnd,
		iconLoop, iconFollow, iconLoopHighlight, iconFollowHighlight,
		iconUndo, iconRedo, iconConfig;
	std::unique_ptr<juce::DrawableButton> playButton, stopButton, beginButton, endButton,
		loopButton, followButton, undoButton, redoButton, configButton;
	std::unique_ptr<juce::TextButton> adsorbButton, gridButton;

	int playCommandID = -1, stopCommandID = -1, beginCommandID = -1, endCommandID = -1,
		loopCommandID = -1, followCommandID = -1,
		backOnStopCommandID = -1, 
		undoCommandID = -1, redoCommandID = -1;

	int commandAdsorb1Beat = -1, commandAdsorb1_2Beat = -1,
		commandAdsorb1_4Beat = -1, commandAdsorb1_6Beat = -1,
		commandAdsorb1_8Beat = -1, commandAdsorb1_12Beat = -1,
		commandAdsorb1_16Beat = -1, commandAdsorb1_24Beat = -1,
		commandAdsorb1_32Beat = -1, commandAdsorbOff = -1;
	int commandGrid1Beat = -1, commandGrid1_2Beat = -1,
		commandGrid1_4Beat = -1, commandGrid1_6Beat = -1,
		commandGrid1_8Beat = -1, commandGrid1_12Beat = -1,
		commandGrid1_16Beat = -1, commandGrid1_24Beat = -1,
		commandGrid1_32Beat = -1;

	juce::ApplicationCommandManager* commandManager = nullptr;
	vocalshaper::ProjectProxy* project = nullptr;
	juce::ReadWriteLock projectLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayBar)
};