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

    //resource
    juce::String logoFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "logo", logoFile, result
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

    this->btNewProj->setWantsKeyboardFocus(false);
    this->btOpenProj->setWantsKeyboardFocus(false);

    this->btNewProj->setLookAndFeel(this->lafs.button);
    this->btOpenProj->setLookAndFeel(this->lafs.button);

    this->addAndMakeVisible(this->btNewProj.get());
    this->addAndMakeVisible(this->btOpenProj.get());

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
    this->teSearchProj->setMultiLine(false);
    this->teSearchProj->setJustification(juce::Justification::centredLeft);
    this->teSearchProj->setClicksOutsideDismissVirtualKeyboard(true);
    this->teSearchProj->setTextToShowWhenEmpty(
        this->tr("lb_SearchEditor"), this->colors.text_search_empty);
    this->addAndMakeVisible(this->teSearchProj.get());
}

SMComponent::~SMComponent()
{
}

void SMComponent::resized()
{
    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);

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
    int searchPosX = leftBarWidth + this->positions.posX_search * (this->getWidth() - leftBarWidth);
    int searchWidth = this->getWidth() - this->sizes.width_search_rightMargin * screenSize.getWidth() - searchPosX;

    //调整搜索框大小
    this->teSearchProj->setBounds(
        searchPosX, titleBarHeight * (1 - this->scales.height_search) / 2,
        searchWidth, titleBarHeight * this->scales.height_search
    );
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
}
