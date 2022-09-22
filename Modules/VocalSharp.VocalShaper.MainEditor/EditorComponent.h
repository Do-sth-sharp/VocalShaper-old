#pragma once
#include <JuceHeader.h>
#include "TopEditor.h"
#include "BottomEditor.h"

class EditorComponent final : public juce::Component
{
public:
	EditorComponent();
	~EditorComponent() override;

	void setTrackOpen(bool trackOpen);
	bool isTrackOpen();

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour stretchableBar;
	}colors;//������ɫ
	struct Size final
	{
		double height_stretchableBar;
	}sizes;//�ؼ���С
	struct Scales final
	{
		double height_top_max;
		double height_top_min;
		double height_top_prefer;
		double height_bottom_max;
		double height_bottom_min;
		double height_bottom_prefer;
	}scales;//�ؼ�����
	struct LookAndFeels final
	{
		juce::LookAndFeel* stretchableBar;
	}lafs;//�ؼ���ʽ

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	juce::Rectangle<int> screenSize;

	void initCommandID();
	void initCommandFunction();
	void initCommandFlagHook();

	std::unique_ptr<TopEditor> topEditor;
	std::unique_ptr<BottomEditor> bottomEditor;

	std::unique_ptr<juce::StretchableLayoutManager> slManager;
	std::unique_ptr<juce::StretchableLayoutResizerBar> slBar;

	bool trackOpenStateChanged = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorComponent)
};