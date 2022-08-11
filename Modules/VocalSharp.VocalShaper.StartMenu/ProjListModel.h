#pragma once
#include <JuceHeader.h>

class ProjListModel final : public juce::ListBoxModel
{
public:
	ProjListModel();
	~ProjListModel() override = default;

	void setScreenSize(juce::Rectangle<int> screenSize);
	void setClickFunc(std::function<void(int, const juce::String&, const juce::String&)> onLeftClick,
		std::function<void(int, const juce::String&, const juce::String&)> onRightClick);

	int getNumRows() override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
	juce::MouseCursor getMouseCursorForRow(int row) override;
	void listBoxItemClicked(int row, const juce::MouseEvent& event) override;

private:
	struct Colors final
	{
		juce::Colour background_list;
		juce::Colour background_listItem;
		juce::Colour text_listItem_name;
		juce::Colour text_listItem_time;
		juce::Colour text_listItem_path;
	}colors;//界面颜色
	struct Sizes final
	{
		double height_fontListItem_name;
		double height_fontListItem_time;
		double height_fontListItem_path;
		double width_listItem_margin;
		double height_listItem_margin;
		double height_listItem_border;
	}sizes;//控件大小

	juce::Rectangle<int> screenSize;

	std::function<void(int, const juce::String&, const juce::String&)> onLeftClick, onRightClick;

	std::function<const juce::String(const juce::String&)> tr;

	std::function<void(int&)> getSizeFunc;
	std::function<void(int, juce::String&)> getNameFunc, getPathFunc, getTimeFunc;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjListModel)
};