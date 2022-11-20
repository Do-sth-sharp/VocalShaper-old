#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackView;

class ColorEditor final : public juce::Component
{
public:
	ColorEditor();
private:
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