#include "TabComponent.h"
#include <libJModule.h>

TabComponent::TabComponent()
	:Component("Main UI Tab Component")
{
    //��ȡ��Ļ���Խӿ�
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
            );

    //���»�ȡ��������
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

    //resource
    juce::String iconMainMenuFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "icon-mainMenu", iconMainMenuFile, result
        );

    //���»�ȡ���˵���ťͼ��
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

    //���¹������˵���ť��ʽ
    jmadf::CallInterface<juce::LookAndFeel*&>(
        "VocalSharp.VocalShaper.LookAndFeelFactory", "GetMainMenuButtonLAF",
        this->lafs.mainMenuButton
        );
    this->lafs.mainMenuButton->setColour(
        juce::TextButton::ColourIds::buttonColourId, this->colors.background_mainMenuButton
    );
    this->lafs.mainMenuButton->setColour(
        juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_mainMenuButton
    );
    this->lafs.mainMenuButton->setColour(
        juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
    );

    //���³�ʼ�����˵���ť
    this->mainMenuButton = std::make_unique<juce::DrawableButton>(
        "bt_MainMenu", juce::DrawableButton::ButtonStyle::ImageOnButtonBackgroundOriginalSize);
    this->mainMenuButton->setImages(this->iconMainMenu.get());
    this->mainMenuButton->setLookAndFeel(this->lafs.mainMenuButton);
    this->mainMenuButton->setWantsKeyboardFocus(false);
    this->mainMenuButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
    this->addAndMakeVisible(this->mainMenuButton.get());

    //���³�ʼ����ǩ�б�
    this->tabList = std::make_unique<TabList>();
    this->addAndMakeVisible(this->tabList.get());
}

void TabComponent::resized()
{
    //�߶Ȼ���
    bool heightChanged = false;
    if (this->heightTemp != this->getHeight()) {
        this->heightTemp = this->getHeight();
        heightChanged = true;
    }

    //�������˵���ť��С
    juce::Rectangle<int> rectMenuButton(
        0, 0,
        this->getHeight(), this->getHeight()
    );
    this->mainMenuButton->setBounds(rectMenuButton);

    //������ǩ�б��С
    this->tabList->setBounds(
        rectMenuButton.getWidth(), 0,
        this->getWidth() - rectMenuButton.getWidth(), this->getHeight()
    );
}

void TabComponent::paint(juce::Graphics& g)
{
    //��䱳��
    g.fillAll(this->colors.background);
}

bool TabComponent::newProj(const juce::String& name, const juce::String& path)
{
    return this->tabList->newProj(name, path);
}

bool TabComponent::copyProj(const juce::String& name, const juce::String& path,
    const juce::String& nameSrc, const juce::String& pathSrc)
{
    return this->tabList->copyProj(name, path, nameSrc, pathSrc);
}

bool TabComponent::openProj(const juce::String& name, const juce::String& path)
{
    return this->tabList->openProj(name, path);
}

bool TabComponent::wannaClose()
{
    return this->tabList->wannaClose();
}

void TabComponent::setCaller(const juce::String& caller)
{
    this->tabList->setCaller(caller);
}
