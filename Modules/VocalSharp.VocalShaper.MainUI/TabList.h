#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TabList final : public juce::Component, public juce::SettableTooltipClient
{
public:
	TabList();
	~TabList() override = default;

	bool newProj(const juce::String& name, const juce::String& path);
	bool copyProj(const juce::String& name, const juce::String& path,
		const juce::String& nameSrc, const juce::String& pathSrc);
	bool openProj(const juce::String& name, const juce::String& path);
	bool isOpenedProj(const juce::String& name, const juce::String& path);
	bool isSavedProj(const juce::String& name, const juce::String& path);
	bool wannaClose();
	void saveCurrent();
	void saveAll();
	void closeOther();
	void copyPath();
	void copyName();
	void copyFullUrl();
	void openPathInSystem();

	void resized() override;
	void paint(juce::Graphics& g) override;
	void mouseMove(const juce::MouseEvent& event) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseEnter(const juce::MouseEvent& event) override;
	void mouseExit(const juce::MouseEvent& event) override;

	void closeButtonClicked();
	void highlightCloseButtonClicked();
	void addButtonClicked();
	void moreButtonClicked();

	void setCaller(const juce::String& caller);

	void listenActions(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type);
	void listenSaved(const vocalshaper::ProjectProxy* proj);

private:
	struct Colors final
	{
		juce::Colour background;

		juce::Colour icon_tabBarButton;
		juce::Colour background_tabBarButton;

		juce::Colour text_tabList;
		juce::Colour background_tabList;
		juce::Colour text_tabList_highlight;
		juce::Colour background_tabList_highlight;

		juce::Colour split;
	}colors;//界面颜色
	struct Sizes final
	{
		double width_tab_max;

		double width_tabBorder;
		double width_tabMarginRight;
		double width_splitLine;
	}sizes;//控件大小
	struct Scales final
	{
		double height_closeButton;
		double height_splitLine;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* tabCloseButton;
		juce::LookAndFeel* tabHighlightCloseButton;
		juce::LookAndFeel* tabBarButton;
	}lafs;//控件样式

	std::function<void(int, vocalshaper::ProjectProxy*&)> getPtrFunc;
	std::function<void(int&)> getCurrentFunc, getSizeFunc;

	std::function<const juce::String(const juce::String&)> tr;
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	bool checkThenClose(int index);
	bool save(int index);
	void refreshCompCache(bool loopFlag = false);
	void refreshComp();
	void showPopupMenu();

	juce::Array<std::pair<int, vocalshaper::ProjectProxy*>> tabShow;
	juce::Array<vocalshaper::ProjectProxy*> tabHide;
	int currentIndex = -1, hoverIndex = -1;

	int heightTemp = 0;
	juce::String caller;
	juce::String projectExtension;

	std::unique_ptr<juce::Drawable> iconClose, iconCloseHighlight, iconAdd, iconMore;
	std::unique_ptr<juce::DrawableButton> btCloseCurrent, btCloseHover;
	std::unique_ptr<juce::DrawableButton> btAdd, btMore;
	std::unique_ptr<juce::Label> lbDefaultFont;

	int closeProjCommandID = -1, closeAllProjCommandID = -1, showStartMenuCommandID = -1,
		closeOtherProjCommandID = -1, saveProjCommandID = -1, saveAllProjCommandID = -1,
		copyPathCommandID = -1, copyNameCommandID = -1, copyFullUrlCommandID = -1,
		openPathInSystemCommandID = -1;
	juce::ApplicationCommandManager* commandManager = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabList)
};