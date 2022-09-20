#include "SMComponent.h"
#include <libJModule.h>

SMComponent::SMComponent()
	: Component("Start Menu Component")
{
    //获取屏幕属性接口
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
            );

    //获取翻译器
    jmadf::CallInterface<std::function<const juce::String(const juce::String&)>&>(
        "WuChang.JMADF.Translates", "GetFunc",
        this->tr
        );

    //以下获取全局设置
    juce::var* config = nullptr;
    bool ok = false;
    jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
        "WuChang.JMADF.GlobalConfigs", "GetReference",
        "config", config, ok
        );
    if (ok && (config != nullptr)) {
        if ((*config)["ProjectExtension"].isString()) {
            this->projectExtension = (*config)["ProjectExtension"].toString();
        }
    }

    //以下获取命令管理器
    jmadf::CallInterface<juce::ApplicationCommandManager*&>(
        "VocalSharp.VocalShaper.CommandManager", "GetCommandManager",
        this->commandManager
        );

    //以下获取命令ID
    jmadf::CallInterface<const juce::String&, int&>(
        "VocalSharp.VocalShaper.CommandManager", "GetCommandID",
        "New Project", this->newProjCommandID
        );
    jmadf::CallInterface<const juce::String&, int&>(
        "VocalSharp.VocalShaper.CommandManager", "GetCommandID",
        "Open Project", this->openProjCommandID
        );

    //以下注册命令回调
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "New Project", [this] {this->newButtonClicked(); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Open Project", [this] {this->openButtonClicked(); }
    );

    //以下获取界面属性
    bool result = false;
    //color
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background", this->colors.background, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "leftBar", this->colors.leftBar, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-button", this->colors.text_button, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-button", this->colors.background_button, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-title", this->colors.text_title, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-search", this->colors.text_search, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-search-empty", this->colors.text_search_empty, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-search", this->colors.background_search, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-search-highlight", this->colors.text_search_highlight, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-search-highlight", this->colors.background_search_highlight, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "caret-search", this->colors.caret_search, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "icon-search", this->colors.icon_search, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-list", this->colors.background_list, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-list-scroller", this->colors.background_list_scroller, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "thumb-list-scroller", this->colors.thumb_list_scroller, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "track-list-scroller", this->colors.track_list_scroller, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-menu-warning", this->colors.text_menu_warning, result
        );

    //size
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-font", this->sizes.height_font, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-leftBar", this->sizes.width_leftBar, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-leftBar-max", this->sizes.width_leftBar_max, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-logo-topMargin", this->sizes.height_logo_topMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-button", this->sizes.width_button, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-button", this->sizes.height_button, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-button-logo-split", this->sizes.height_button_logo_split, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-button-split", this->sizes.height_button_split, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-titleBar", this->sizes.height_titleBar, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-titleBar-max", this->sizes.height_titleBar_max, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-title-leftMargin", this->sizes.width_title_leftMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-search-rightMargin", this->sizes.width_search_rightMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-title-search-split", this->sizes.width_title_search_split, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-list-topMargin", this->sizes.height_list_topMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-list-bottomMargin", this->sizes.height_list_bottomMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-list-leftMargin", this->sizes.width_list_leftMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-list-rightMargin", this->sizes.width_list_rightMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-listItem", this->sizes.height_listItem, result
        );
    //position
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "position", "posX-search", this->positions.posX_search, result
        );
    //scale
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "width-logo", this->scales.width_logo, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-title", this->scales.height_title, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-search", this->scales.height_search, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-icon-search", this->scales.height_icon_search, result
        );

    //resource
    juce::String logoFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "logo", logoFile, result
        );
    juce::String iconSearchFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "icon-search", iconSearchFile, result
        );
	
    //以下获取头图
    size_t logoSize = 0;
    void* logoPtr = nullptr;
    juce::String logoPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
        .getParentDirectory().getFullPathName() + logoFile;

    jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
        "WuChang.JMADF.DynamicRC",
        "GetRC",
        logoPath, std::pair<size_t&, void*&>(logoSize, logoPtr)
        );

    this->logoImage = juce::ImageFileFormat::loadFrom(logoPtr, logoSize);

    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);

    //以下构建按钮样式
    jmadf::CallInterface<int, juce::LookAndFeel*&>(
        "VocalSharp.VocalShaper.LookAndFeelFactory", "GetStartMenuButtonLAF",
        screenSize.getHeight() * this->sizes.height_font, this->lafs.button
        );
    this->lafs.button->setColour(
        juce::TextButton::ColourIds::buttonColourId, this->colors.background_button);
    this->lafs.button->setColour(
        juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_button);
    this->lafs.button->setColour(
        juce::TextButton::ColourIds::textColourOffId, this->colors.text_button);
    this->lafs.button->setColour(
        juce::TextButton::ColourIds::textColourOnId, this->colors.text_button);
    this->lafs.button->setColour(
        juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
    );

    //以下初始化按钮
    this->btNewProj = std::make_unique<juce::TextButton>(
        this->tr("bt_NewProject"), this->tr("tip_NewProject"));
    this->btOpenProj = std::make_unique<juce::TextButton>(
        this->tr("bt_OpenProject"), this->tr("tip_OpenProject"));

    this->btNewProj->setCommandToTrigger(this->commandManager, this->newProjCommandID, false);
    this->btOpenProj->setCommandToTrigger(this->commandManager, this->openProjCommandID, false);

    this->btNewProj->setWantsKeyboardFocus(false);
    this->btOpenProj->setWantsKeyboardFocus(false);

    this->btNewProj->setLookAndFeel(this->lafs.button);
    this->btOpenProj->setLookAndFeel(this->lafs.button);

    this->addAndMakeVisible(this->btNewProj.get());
    this->addAndMakeVisible(this->btOpenProj.get());

    //以下加载搜索图标
    size_t iconSearchSize = 0;
    void* iconSearchPtr = nullptr;
    juce::String iconSearchPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
        .getParentDirectory().getFullPathName() + iconSearchFile;

    jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
        "WuChang.JMADF.DynamicRC",
        "GetRC",
        iconSearchPath, std::pair<size_t&, void*&>(iconSearchSize, iconSearchPtr)
        );
    if (iconSearchPtr) {
        juce::String iconSearchStr((char*)iconSearchPtr, iconSearchSize);
        auto ptrXml = juce::XmlDocument::parse(iconSearchStr);
        if (ptrXml) {
            this->iconSearch = juce::Drawable::createFromSVG(*ptrXml);
        }
    }

    //以下构建搜索框样式
    jmadf::CallInterface<juce::LookAndFeel*&>(
        "VocalSharp.VocalShaper.LookAndFeelFactory", "GetStartMenuTextEditorLAF",
        this->lafs.textEditor
        );
    this->lafs.textEditor->setColour(
        juce::TextEditor::ColourIds::textColourId, this->colors.text_search
    );
    this->lafs.textEditor->setColour(
        juce::TextEditor::ColourIds::backgroundColourId, this->colors.background_search
    );
    this->lafs.textEditor->setColour(
        juce::TextEditor::ColourIds::highlightedTextColourId, this->colors.text_search_highlight
    );
    this->lafs.textEditor->setColour(
        juce::TextEditor::ColourIds::highlightColourId, this->colors.background_search_highlight
    );
    this->lafs.textEditor->setColour(
        juce::TextEditor::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
    );
    this->lafs.textEditor->setColour(
        juce::TextEditor::ColourIds::focusedOutlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
    );
    this->lafs.textEditor->setColour(
        juce::TextEditor::ColourIds::shadowColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
    );
    this->lafs.textEditor->setColour(
        juce::CaretComponent::ColourIds::caretColourId, this->colors.caret_search
    );

    //以下初始化搜索框
    this->teSearchProj = std::make_unique<juce::TextEditor>();
    this->teSearchProj->setLookAndFeel(this->lafs.textEditor);
    juce::Font searchFont = this->teSearchProj->getFont();
    searchFont.setHeight(screenSize.getHeight()* this->sizes.height_font);
    this->teSearchProj->setFont(searchFont);
    this->teSearchProj->setHasFocusOutline(false);
    this->teSearchProj->setMultiLine(false);
    this->teSearchProj->setJustification(juce::Justification::centredLeft);
    this->teSearchProj->setClicksOutsideDismissVirtualKeyboard(true);
    this->teSearchProj->setPopupMenuEnabled(false);
    this->teSearchProj->setTextToShowWhenEmpty(
        this->tr("lb_SearchEditor"), this->colors.text_search_empty);
    this->teSearchProj->onTextChange = [this] {
        this->filterChanged();
    };
    this->addAndMakeVisible(this->teSearchProj.get());

    //以下构建项目列表样式
    jmadf::CallInterface<juce::LookAndFeel*&>(
        "VocalSharp.VocalShaper.LookAndFeelFactory", "GetStartMenuListBoxLAF",
        this->lafs.listBox
        );
    this->lafs.listBox->setColour(
        juce::ListBox::ColourIds::backgroundColourId, this->colors.background_list
    );
    this->lafs.listBox->setColour(
        juce::ListBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
    );
    this->lafs.listBox->setColour(
        juce::ScrollBar::ColourIds::backgroundColourId, this->colors.background_list_scroller
    );
    this->lafs.listBox->setColour(
        juce::ScrollBar::ColourIds::thumbColourId, this->colors.thumb_list_scroller
    );
    this->lafs.listBox->setColour(
        juce::ScrollBar::ColourIds::trackColourId, this->colors.track_list_scroller
    );

    //以下初始化项目列表
    this->lstProj = std::make_unique<juce::ListBox>();
    this->lstProj->setWantsKeyboardFocus(false);
    this->lstProj->setLookAndFeel(this->lafs.listBox);
    this->lstProj->setRowSelectedOnMouseDown(false);
    this->lstProj->setRowHeight(
        this->sizes.height_listItem * screenSize.getHeight()
    );
    this->addAndMakeVisible(this->lstProj.get());

    //以下初始化项目列表模型
    this->lstModel = std::make_unique<ProjListModel>();
    this->lstModel->setScreenSize(screenSize);
    this->lstModel->setClickFunc(
        [this](int row, const juce::String& name, const juce::String& path) {
            this->listItemLeftClicked(row, name, path);
        },
        [this](int row, const juce::String& name, const juce::String& path) {
            this->listItemRightClicked(row, name, path);
        }
        );
    this->refreshList();

    //空白处获取焦点
    this->setWantsKeyboardFocus(true);
}

SMComponent::~SMComponent()
{
    this->lstProj->setModel(nullptr);
}

void SMComponent::resized()
{
    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);
    if (screenSize != this->screenSizeTemp) {
        //屏幕大小变化
        this->screenSizeTemp = screenSize;

        juce::Font searchFont = this->teSearchProj->getFont();
        searchFont.setHeight(screenSize.getHeight() * this->sizes.height_font);
        this->teSearchProj->applyFontToAllText(searchFont);

        this->lstModel->setScreenSize(screenSize);
        this->lstProj->setRowHeight(
            this->sizes.height_listItem * screenSize.getHeight()
        );
    }

    //计算按钮大小
    int leftBarWidth =
        (int)std::min(this->sizes.width_leftBar * screenSize.getWidth(), this->sizes.width_leftBar_max);
    double logoScale = (leftBarWidth * this->scales.width_logo) / (double)(this->logoImage.getWidth());
    int logoPlaceHeight = (screenSize.getHeight() * this->sizes.height_logo_topMargin) + this->logoImage.getHeight() * logoScale;
    int buttonWidth = leftBarWidth * this->sizes.width_button;
    int buttonHeight = screenSize.getHeight() * this->sizes.height_button;
    int buttonLogoSplitHeight = screenSize.getHeight() * this->sizes.height_button_logo_split;
    int buttonSplitHeight = screenSize.getHeight() * this->sizes.height_button_split;
    int buttonWCenter = leftBarWidth / 2;
    int buttonHPos = logoPlaceHeight + buttonLogoSplitHeight;

    //调整按钮大小位置
    this->btNewProj->setBounds(
        buttonWCenter - buttonWidth / 2, buttonHPos,
        buttonWidth, buttonHeight
    );
    this->btOpenProj->setBounds(
        buttonWCenter - buttonWidth / 2, buttonHPos + buttonHeight + buttonSplitHeight,
        buttonWidth, buttonHeight
    );

    //计算搜索框大小
    int titleBarHeight =
        (int)std::min(screenSize.getHeight() * this->sizes.height_titleBar, this->sizes.height_titleBar_max);
    int iconSearchWidth = 0;
    if (this->iconSearch) {
        iconSearchWidth = titleBarHeight * this->scales.height_search;
    }
    int searchPosX = leftBarWidth + this->positions.posX_search * (this->getWidth() - leftBarWidth) + iconSearchWidth;
    int searchWidth = this->getWidth() - this->sizes.width_search_rightMargin * screenSize.getWidth() - searchPosX;

    //调整搜索框大小
    this->teSearchProj->setBounds(
        searchPosX, titleBarHeight * (1 - this->scales.height_search) / 2,
        searchWidth, titleBarHeight * this->scales.height_search
    );

    //计算列表大小
    int listPosX = leftBarWidth + this->sizes.width_list_leftMargin * screenSize.getWidth();
    int listPosY = titleBarHeight + this->sizes.height_list_topMargin * screenSize.getHeight();
    int listWidth = this->getWidth() - listPosX - this->sizes.width_list_rightMargin * screenSize.getWidth();
    int listHeight = this->getHeight() - listPosY - this->sizes.height_list_bottomMargin * screenSize.getHeight();

    //调整列表大小
    this->lstProj->setBounds(
        listPosX, listPosY,
        listWidth, listHeight
    );
    this->lstProj->scrollToEnsureRowIsOnscreen(0);
}

void SMComponent::paint(juce::Graphics& g)
{
    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);
	
    //填充背景
    g.fillAll(this->colors.background);
	
    //左侧条带宽度
    int leftBarWidth =
        (int)std::min(this->sizes.width_leftBar * screenSize.getWidth(), this->sizes.width_leftBar_max);
	
    //填充左侧条带背景
    g.setColour(this->colors.leftBar);
    g.fillRect(0, 0, leftBarWidth, this->getHeight());

    //绘制左上头图
    double logoScale = (leftBarWidth * this->scales.width_logo) / (double)(this->logoImage.getWidth());
    juce::Rectangle<int> logoRect(
        (leftBarWidth / 2.0) * (1 - this->scales.width_logo), screenSize.getHeight() * this->sizes.height_logo_topMargin,
        leftBarWidth * this->scales.width_logo, this->logoImage.getHeight() * logoScale
    );
    g.drawImageWithin(
        this->logoImage,
        logoRect.getX(), logoRect.getY(),
        logoRect.getWidth(), logoRect.getHeight(),
        juce::RectanglePlacement::Flags::centred
    );

    //计算标题栏高度
    int titleBarHeight =
        (int)std::min(screenSize.getHeight() * this->sizes.height_titleBar, this->sizes.height_titleBar_max);

    //绘标题栏高度辅助线
    //g.setColour(juce::Colours::white);
    //g.drawLine(leftBarWidth, titleBarHeight, this->getWidth(), titleBarHeight);

    //绘制标题
    g.setColour(this->colors.text_title);

    juce::Font titleFont = g.getCurrentFont();
    titleFont.setHeight(titleBarHeight * this->scales.height_title);
    g.setFont(titleFont);

    juce::String titleStr = this->tr("lb_RecentProject");
    int titlePosX = leftBarWidth + screenSize.getWidth() * this->sizes.width_title_leftMargin;
    int searchPosX = leftBarWidth + this->positions.posX_search * (this->getWidth() - leftBarWidth);
    int titleWidth =
        (int)std::min((double)titleFont.getStringWidth(titleStr),
            searchPosX - titlePosX - this->sizes.width_title_search_split * screenSize.getWidth());
    juce::Rectangle<int> titleRect(
        titlePosX, titleBarHeight * (1 - this->scales.height_title) / 2,
        titleWidth, titleBarHeight * this->scales.height_title
    );

    g.drawFittedText(
        titleStr, titleRect,
        juce::Justification::centred, 1
    );

    //绘制搜索图标
    if (this->iconSearch) {
        int iconSearchHeight = titleBarHeight * this->scales.height_search;
        int iconSearchWidth = iconSearchHeight;
        int iconSearchPosX = leftBarWidth + this->positions.posX_search * (this->getWidth() - leftBarWidth);
        int iconSearchPosY = titleBarHeight * (1 - this->scales.height_search) / 2;
        juce::Rectangle<int> rectIconSearch(
            iconSearchPosX, iconSearchPosY, iconSearchWidth, iconSearchHeight
        );
        juce::Rectangle<float> rectDrawableIconSearch(
            iconSearchPosX + iconSearchWidth * (1 - this->scales.height_icon_search) / 2,
            iconSearchPosY + iconSearchHeight * (1 - this->scales.height_icon_search) / 2,
            iconSearchWidth * this->scales.height_icon_search,
            iconSearchHeight * this->scales.height_icon_search
        );

        g.setColour(this->colors.background_search);
        g.fillRect(rectIconSearch);
        this->iconSearch->replaceColour(juce::Colours::black, this->colors.icon_search);
        this->iconSearch->drawWithin(
            g, rectDrawableIconSearch,
            juce::RectanglePlacement::centred | juce::RectanglePlacement::stretchToFit,
            1.0f
        );
    }
}

void SMComponent::setCaller(const juce::String& caller)
{
    this->caller = caller;
}

void SMComponent::listItemLeftClicked(int row, const juce::String& name, const juce::String& path)
{
    if (this->openProj(name, path)) {
        jmadf::CallInterface<int>(
            "VocalSharp.VocalShaper.ProjectHistory", "Open",
            row
            );
        this->clearFilter();
        return;
    }
    else {
        juce::AlertWindow::showMessageBox(
            juce::MessageBoxIconType::WarningIcon, this->tr("bt_OpenProject"),
            this->tr("tip_CouldNotOpen"), this->tr("bt_OK")
        );
        jmadf::CallInterface<int>(
            "VocalSharp.VocalShaper.ProjectHistory", "Remove",
            row
            );
    }
    this->refreshList();
}

void SMComponent::listItemRightClicked(int row, const juce::String& name, const juce::String& path)
{
    juce::PopupMenu menu;
    menu.addSectionHeader(name);
    menu.addItem(1, this->tr("bt_Open"));
    menu.addItem(2, this->tr("bt_OpenAsCopy"));
    menu.addSeparator();
    menu.addItem(3, this->tr("bt_CopyName"));
    menu.addItem(4, this->tr("bt_CopyPath"));
    menu.addItem(5, this->tr("bt_CopyFullUrl"));
    menu.addItem(6, this->tr("bt_OpenDirectory"));
    menu.addSeparator();
    menu.addColouredItem(7, this->tr("bt_Remove"), this->colors.text_menu_warning);
    int result = menu.show();
    switch (result)
    {
    case 1:
    {
        //打开
        this->listItemLeftClicked(row, name, path);
        break;
    }
    case 2:
    {
        //创建副本并打开
        juce::FileChooser fileChooser(
            this->tr("bt_OpenAsCopy"),
            juce::File::getCurrentWorkingDirectory(), "*" + this->projectExtension
        );
        if (fileChooser.showDialog(
            juce::FileBrowserComponent::FileChooserFlags::canSelectFiles |
            juce::FileBrowserComponent::FileChooserFlags::doNotClearFileNameOnRootChange |
            juce::FileBrowserComponent::FileChooserFlags::saveMode,
            nullptr
        )) {
            juce::File file = fileChooser.getResult();
            if (file.exists()) {
                juce::AlertWindow::showMessageBox(
                    juce::MessageBoxIconType::WarningIcon, this->tr("bt_OpenAsCopy"),
                    this->tr("tip_FileExists"), this->tr("bt_OK")
                );
                return;
            }
            juce::File dir = file.getParentDirectory();
            if (dir.getNumberOfChildFiles(
                juce::File::TypesOfFileToFind::findFilesAndDirectories) > 0) {
                juce::AlertWindow::showMessageBox(
                    juce::MessageBoxIconType::WarningIcon, this->tr("bt_OpenAsCopy"),
                    this->tr("tip_PathNotEmpty"), this->tr("bt_OK")
                );
                return;
            }
            dir.setAsCurrentWorkingDirectory();
            juce::String pathDst = dir.getFullPathName();
            juce::String nameDst = file.getFileNameWithoutExtension();
            if (!this->copyProj(nameDst, pathDst, name, path)) {
                juce::AlertWindow::showMessageBox(
                    juce::MessageBoxIconType::WarningIcon, this->tr("bt_OpenAsCopy"),
                    this->tr("tip_CouldNotCreate"), this->tr("bt_OK")
                );
                return;
            }
            jmadf::CallInterface<const juce::String&, const juce::String&>(
                "VocalSharp.VocalShaper.ProjectHistory", "Add",
                nameDst, pathDst
                );
            this->clearFilter();
        }
        break;
    }
    case 3:
    {
        //复制名称
        juce::SystemClipboard::copyTextToClipboard(name);
        break;
    }
    case 4:
    {
        //复制目录
        juce::SystemClipboard::copyTextToClipboard(path);
        break;
    }
    case 5:
    {
        //复制完整路径
        juce::String url = juce::File(path + "/"
            + name + this->projectExtension).getFullPathName();
        juce::SystemClipboard::copyTextToClipboard(url);
        break;
    }
    case 6:
    {
        //打开目录
        juce::Process::openDocument(path, juce::String());
        break;
    }
    case 7:
    {
        //移除
        jmadf::CallInterface<int>(
            "VocalSharp.VocalShaper.ProjectHistory", "Remove",
            row
            );
        this->clearFilter();
        break;
    }
    default:
        break;
    }
}

void SMComponent::newButtonClicked()
{
    juce::FileChooser fileChooser(
        this->tr("bt_NewProject"),
        juce::File::getCurrentWorkingDirectory(), "*" + this->projectExtension
    );
    if (fileChooser.showDialog(
        juce::FileBrowserComponent::FileChooserFlags::canSelectFiles |
        juce::FileBrowserComponent::FileChooserFlags::doNotClearFileNameOnRootChange |
        juce::FileBrowserComponent::FileChooserFlags::saveMode,
        nullptr
    )) {
        juce::File file = fileChooser.getResult();
        if (file.exists()) {
            juce::AlertWindow::showMessageBox(
                juce::MessageBoxIconType::WarningIcon, this->tr("bt_NewProject"),
                this->tr("tip_FileExists"), this->tr("bt_OK")
            );
            return;
        }
        juce::File dir = file.getParentDirectory();
        if (dir.getNumberOfChildFiles(
            juce::File::TypesOfFileToFind::findFilesAndDirectories) > 0) {
            juce::AlertWindow::showMessageBox(
                juce::MessageBoxIconType::WarningIcon, this->tr("bt_NewProject"),
                this->tr("tip_PathNotEmpty"), this->tr("bt_OK")
            );
            return;
        }
        dir.setAsCurrentWorkingDirectory();
        juce::String path = dir.getFullPathName();
        juce::String name = file.getFileNameWithoutExtension();
        if (!this->newProj(name, path)) {
            juce::AlertWindow::showMessageBox(
                juce::MessageBoxIconType::WarningIcon, this->tr("bt_NewProject"),
                this->tr("tip_CouldNotCreate"), this->tr("bt_OK")
            );
            return;
        }
        jmadf::CallInterface<const juce::String&, const juce::String&>(
            "VocalSharp.VocalShaper.ProjectHistory", "Add",
            name, path
            );
        this->clearFilter();
    }
}

void SMComponent::openButtonClicked()
{
    juce::FileChooser fileChooser(
        this->tr("bt_OpenProject"),
        juce::File::getCurrentWorkingDirectory(), "*" + this->projectExtension
    );
    if (fileChooser.showDialog(
        juce::FileBrowserComponent::FileChooserFlags::canSelectFiles |
        juce::FileBrowserComponent::FileChooserFlags::openMode,
        nullptr
    )) {
        juce::File file = fileChooser.getResult();
        juce::File dir = file.getParentDirectory();
        dir.setAsCurrentWorkingDirectory();

        juce::String path = dir.getFullPathName();
        juce::String name = file.getFileNameWithoutExtension();
        this->openProjFromUrl(name, path);
    }
}

void SMComponent::filterChanged()
{
    jmadf::CallInterface<const juce::String&>(
        "VocalSharp.VocalShaper.ProjectHistory", "SetFilter",
        this->teSearchProj->getText()
        );
    this->refreshList();
}

void SMComponent::openProjFromUrl(const juce::String& name, const juce::String& path)
{
    if (!this->openProj(name, path)) {
        juce::AlertWindow::showMessageBox(
            juce::MessageBoxIconType::WarningIcon, this->tr("bt_OpenProject"),
            this->tr("tip_CouldNotOpen"), this->tr("bt_OK")
        );
        return;
    }
    jmadf::CallInterface<const juce::String&, const juce::String&>(
        "VocalSharp.VocalShaper.ProjectHistory", "Add",
        name, path
        );
    this->clearFilter();
}

bool SMComponent::newProj(const juce::String& name, const juce::String& path)
{
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
        this->caller, "NewProject",
        name, path, result
        );
    return result;
}

bool SMComponent::copyProj(const juce::String& name, const juce::String& path,
    const juce::String& nameSrc, const juce::String& pathSrc)
{
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
        this->caller, "CopyProject",
        name, path, nameSrc, pathSrc, result
        );
    return result;
}

bool SMComponent::openProj(const juce::String& name, const juce::String& path)
{
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
        this->caller, "OpenProject",
        name, path, result
        );
    return result;
}

void SMComponent::clearFilter()
{
    this->teSearchProj->setText(juce::String());
    this->filterChanged();
}

void SMComponent::refreshList()
{
    this->lstProj->setModel(nullptr);
    this->lstProj->setModel(this->lstModel.get());
    this->lstProj->scrollToEnsureRowIsOnscreen(0);
}
