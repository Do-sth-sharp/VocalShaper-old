﻿#pragma once
#include <JuceHeader.h>
#include "ProjListModel.h"

class SMComponent final : public juce::Component
{
public:
	SMComponent();
	~SMComponent() override;
	
	void paint(juce::Graphics& g) override;
	void resized() override;

	void setCaller(const juce::String& caller);

	void listItemClicked(int row, const juce::String& name, const juce::String& path);
	void newButtonClicked();
	void openButtonClicked();
	void filterChanged();
	
private:
	struct Colors final
	{
		juce::Colour background;
		juce::Colour leftBar;
		juce::Colour text_button;
		juce::Colour background_button;
		juce::Colour text_title;
		juce::Colour text_search;
		juce::Colour text_search_empty;
		juce::Colour background_search;
		juce::Colour text_search_highlight;
		juce::Colour background_search_highlight;
		juce::Colour caret_search;
		juce::Colour icon_search;
		juce::Colour background_list;
		juce::Colour background_list_scroller;
		juce::Colour thumb_list_scroller;
		juce::Colour track_list_scroller;
	}colors;//界面颜色
	struct Sizes final
	{
		double height_font;
		double width_leftBar;
		double width_leftBar_max;
		double height_logo_topMargin;
		double width_button;
		double height_button;
		double height_button_logo_split;
		double height_button_split;
		double height_titleBar;
		double height_titleBar_max;
		double width_title_leftMargin;
		double width_search_rightMargin;
		double width_title_search_split;
		double height_list_topMargin;
		double height_list_bottomMargin;
		double width_list_leftMargin;
		double width_list_rightMargin;
		double height_listItem;
	}sizes;//控件大小
	struct Positions final
	{
		double posX_search;
	}positions;//控件位置
	struct Scales final
	{
		double width_logo;
		double height_title;
		double height_search;
		double height_icon_search;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* button;
		juce::LookAndFeel* textEditor;
		juce::LookAndFeel* listBox;
	}lafs;//控件样式
	
	juce::Image logoImage;
	std::unique_ptr<juce::Drawable> iconSearch;
	std::unique_ptr<juce::TextButton> btNewProj, btOpenProj;
	std::unique_ptr<juce::TextEditor> teSearchProj;
	std::unique_ptr<juce::ListBox> lstProj;
	std::unique_ptr<ProjListModel> lstModel;

	juce::Rectangle<int> screenSizeTemp;

	juce::String caller;

	std::function<const juce::String(const juce::String&)> tr;
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	bool newProj(const juce::String& name, const juce::String& path);
	bool openProj(const juce::String& name, const juce::String& path);

	void refreshList();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SMComponent)
};