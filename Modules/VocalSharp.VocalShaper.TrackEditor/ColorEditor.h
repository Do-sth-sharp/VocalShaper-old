#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackView;

class ColorEditor final : public juce::Component
{
public:
	ColorEditor();

	void resized() override;
	void paint(juce::Graphics& g) override;

	void setCurrentColor(juce::Colour currentColor);
	void setSingerColor(bool hasSingerColor, juce::Colour color = juce::Colour());

private:
	struct Colors final
	{
		juce::Colour background_colorEditor;

		juce::Colour text_colorEditorButton;
		juce::Colour background_colorEditorButton;

		juce::Colour splitLine_colorEditor;

		juce::Colour text_colorEditorTitle;
	}colors;//界面颜色
	struct Size final
	{
		double height_colorEditorMarginTop;
		double height_colorEditorMarginBottom;
		double width_colorEditorMarginLeft;
		double width_colorEditorMarginRight;

		double width_colorEditorButton;

		double width_colorEditorSplitLine;
		double height_colorEditorSplitLine;

		double width_colorEditorSplit;

		double height_colorEditorTitleFont;
		double width_colorEditorTitleLeftMargin;
	}sizes;//控件大小
	struct Positions final
	{
		double posY_colorEditorButton;

		double posX_colorEditorButton;

		double posX_colorEditorSplit;
		double posY_colorEditorSplitLine;

		double posY_colorEditorLeftSplit1;
		double posY_colorEditorLeftSplit2;
		double posY_colorEditorLeftSplit3;
	}positions;//控件位置
	struct Scale final
	{
		double height_colorEditorButton;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		std::unique_ptr<juce::LookAndFeel> button;
	}lafs;//控件样式

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	std::function<const juce::String(const juce::String&)> tr;

	std::unique_ptr<juce::TextButton> okButton = nullptr;

	juce::Colour currentColor;
	bool hasSinger = false;
	juce::Colour singerColor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditor)
};

class ColorEditorCallOutBox final
{
public:
	ColorEditorCallOutBox() = delete;
	ColorEditorCallOutBox(TrackView* parent);

	void resize(int width, int height);
	void setArrowSize(float arrowWidth);
	void setBorderSize(int borderSize);
	void setCornerSize(float cornerSize);

	void showAt(juce::Rectangle<int> rect);
	void close();

	void setCurrentColor(juce::Colour currentColour);
	void setSingerColor(bool hasSingerColor, juce::Colour color = juce::Colour());

private:
	class ColorEditorCallOutBoxCallback final
		: public juce::ModalComponentManager::Callback
	{
	public:
		ColorEditorCallOutBoxCallback() = delete;
		ColorEditorCallOutBoxCallback(ColorEditorCallOutBox* manager);

		void setArrowSize(float arrowWidth);

		void showAt(juce::Rectangle<int> rect);
		void close();

	public:
		void modalStateFinished(int) override;

	private:
		std::unique_ptr<juce::CallOutBox> callOutBox = nullptr;

		TrackView* parent = nullptr;
		ColorEditorCallOutBox* manager = nullptr;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditorCallOutBoxCallback)
	};

private:
	struct Colors final
	{
		juce::Colour background_colorEditor;
		juce::Colour border_colorEditor;
	}colors;//界面颜色
	struct LookAndFeels final
	{
		std::unique_ptr<juce::LookAndFeel> callOutBox;
	}lafs;//控件样式

	friend class ColorEditorCallOutBoxCallback;

	TrackView* parent = nullptr;
	float arrowWidth = 16.f;
	int borderSize = 20;
	float cornerSize = 9.f;

	std::unique_ptr<ColorEditor> colorEditor = nullptr;
	std::unique_ptr<ColorEditorCallOutBoxCallback> callback = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditorCallOutBox)
};