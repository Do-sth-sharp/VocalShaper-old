#include "TabComponent.h"
#include <libJModule.h>

TabComponent::TabComponent()
	:Component("Main UI Tab Component")
{
    //获取屏幕属性接口
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
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
        "main", "color", "icon-mainMenuButton", this->colors.icon_mainMenuButton, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-mainMenuButton", this->colors.background_mainMenuButton, result
        );

    //size
    //position
    //scale
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "icon-mainMenuButton", this->scales.icon_mainMenuButton, result
        );

    //resource
    juce::String iconMainMenuFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "icon-mainMenu", iconMainMenuFile, result
        );

    //以下获取主菜单按钮图标
    size_t iconMainMenuSize = 0;
    void* iconMainMenuPtr = nullptr;
    juce::String iconMainMenuPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
        .getParentDirectory().getFullPathName() + iconMainMenuFile;

    jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
        "WuChang.JMADF.DynamicRC",
        "GetRC",
        iconMainMenuPath, std::pair<size_t&, void*&>(iconMainMenuSize, iconMainMenuPtr)
        );
    if (iconMainMenuPtr) {
        juce::String iconMainMenuStr((char*)iconMainMenuPtr, iconMainMenuSize);
        auto ptrXml = juce::XmlDocument::parse(iconMainMenuStr);
        if (ptrXml) {
            this->iconMainMenu = juce::Drawable::createFromSVG(*ptrXml);
            if (this->iconMainMenu) {
                this->iconMainMenu->replaceColour(
                    juce::Colours::black, this->colors.icon_mainMenuButton
                );
            }
        }
    }

    //以下构建主菜单按钮样式
    jmadf::CallInterface<juce::LookAndFeel*&>(
        "VocalSharp.VocalShaper.LookAndFeelFactory", "GetMainMenuButtonLAF",
        this->lafs.mainMenuButton
        );
    this->lafs.mainMenuButton->setColour(
        juce::DrawableButton::ColourIds::backgroundColourId, this->colors.background_mainMenuButton
    );
    this->lafs.mainMenuButton->setColour(
        juce::DrawableButton::ColourIds::backgroundOnColourId, this->colors.background_mainMenuButton
    );

    //以下初始化主菜单按钮
    this->mainMenuButton = std::make_unique<juce::DrawableButton>(
        "bt_MainMenu", juce::DrawableButton::ButtonStyle::ImageRaw);
    this->mainMenuButton->setImages(this->iconMainMenu.get());
    this->mainMenuButton->setLookAndFeel(this->lafs.mainMenuButton);
    this->mainMenuButton->setWantsKeyboardFocus(false);
    this->mainMenuButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
    this->addAndMakeVisible(this->mainMenuButton.get());
}

void TabComponent::resized()
{
    //高度缓存
    bool heightChanged = false;
    if (this->heightTemp != this->getHeight()) {
        this->heightTemp = this->getHeight();
        heightChanged = true;
    }

    //调整主菜单按钮大小
    juce::Rectangle<int> rectMenuButton(
        0, 0,
        this->getHeight(), this->getHeight()
    );
    this->mainMenuButton->setBounds(rectMenuButton);

    //调整主菜单按钮图标大小
    if (heightChanged) {
        juce::Rectangle<int> areaButton(
            0, 0,
            rectMenuButton.getWidth(), rectMenuButton.getHeight()
        );
        juce::Rectangle<float> iconBoundsToFit(
            areaButton.getWidth() * (1 - this->scales.icon_mainMenuButton) / 2,
            areaButton.getHeight() * (1 - this->scales.icon_mainMenuButton) / 2,
            areaButton.getWidth() * this->scales.icon_mainMenuButton,
            areaButton.getHeight() * this->scales.icon_mainMenuButton
        );
        this->iconMainMenu->setTransformToFit(
            iconBoundsToFit,
            juce::RectanglePlacement::centred);
        this->mainMenuButton->setImages(this->iconMainMenu.get());
    }
}

void TabComponent::paint(juce::Graphics& g)
{
    //填充背景
    g.fillAll(this->colors.background);
}