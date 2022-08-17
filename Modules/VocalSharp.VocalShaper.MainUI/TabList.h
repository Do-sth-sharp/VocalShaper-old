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
	std::function<void(int, vocalshaper::Project*&)> getPtrFunc;
	std::function<void(int&)> getCurrentFunc, getSizeFunc;

	std::function<const juce::String(const juce::String&)> tr;
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	bool checkThenClose(int index);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabList)
};