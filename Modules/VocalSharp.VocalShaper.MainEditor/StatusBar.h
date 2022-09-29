#pragma once
#include <JuceHeader.h>

class StatusBar final : public juce::Component
{
public:
	StatusBar();
	~StatusBar() override = default;

	void selectNoteEditorPlugin();
	void selectNoteEditionAdditionPlugin();

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Colors final
	{
		juce::Colour background_statusBar;
		juce::Colour border_statusBar;

		juce::Colour text_statusBarTextButton;
		juce::Colour background_statusBarTextButton;
		juce::Colour text_statusBarTextButton_highlight;
		juce::Colour background_statusBarTextButton_highlight;

		juce::Colour icon_statusBarButton;
		juce::Colour background_statusBarButton;
		juce::Colour icon_statusBarButton_highlight;
		juce::Colour background_statusBarButton_highlight;

		juce::Colour split_statusBar;
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
		juce::LookAndFeel* statusButton;
		juce::LookAndFeel* statusTextButton;
	}lafs;//控件样式

	void initCommandID();
	void initCommandFunction();
	void initCommandFlagHook();

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	std::function<const juce::String(const juce::String&)> tr;

	std::unique_ptr<juce::TextButton> mixtureEditorButton, additionEditorButton;
	std::unique_ptr<juce::TextButton> noteEditorPluginButton, noteEditorAdditionPluginButton;
	std::unique_ptr<juce::DrawableButton> noteEditorButton, mixEditorButton, scriptEditorButton;
	std::unique_ptr<juce::Drawable> iconNoteEditor, iconNoteEditorHighlight,
		iconMixEditor, iconMixEditorHighlight,
		iconScriptEditor, iconScriptEditorHighlight;

	int showMixtureEditorCommandID = -1, showAdditionEditorCommandID = -1,
		noteEditorCommandID = -1, mixEditorCommandID = -1, scriptEditorCommandID = -1,
		noteEditorPluginCommandID = -1, noteEditorAdditionPluginCommandID = -1;
	juce::ApplicationCommandManager* commandManager = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBar)
};