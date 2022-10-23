﻿#pragma once
#include <JuceHeader.h>

class ProjListModel final : public juce::ListBoxModel
{
public:
	ProjListModel();
	~ProjListModel() override = default;

	void setScreenSize(juce::Rectangle<int> screenSize);
	void setClickFunc(const std::function<void(int, const juce::String&, const juce::String&)>& onLeftClick,
		const std::function<void(int, const juce::String&, const juce::String&)>& onRightClick);
	void setCheckOpenendFunc(const std::function<bool(const juce::String&, const juce::String&)>& checkOpenedFunc);

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
		juce::Colour light_listItemStarted;
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
	struct Scales final
	{
		double height_listItemStarted_light;
	}scales;//控件缩放

	juce::Rectangle<int> screenSize;

	std::function<void(int, const juce::String&, const juce::String&)> onLeftClick, onRightClick;
	std::function<bool(const juce::String&, const juce::String&)> checkOpenedFunc;

	std::function<const juce::String(const juce::String&)> tr;

	std::function<void(int&)> getSizeFunc;
	std::function<void(int, juce::String&)> getNameFunc, getPathFunc, getTimeFunc;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjListModel)
};