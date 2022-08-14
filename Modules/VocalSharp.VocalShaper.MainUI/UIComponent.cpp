#include "UIComponent.h"
#include <libJModule.h>

UIComponent::UIComponent()
    : Component("Main UI Component")
{
    //��ȡ��Ļ���Խӿ�
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
            );

    //���»�ȡ��������
    bool result = false;
    //color

    //size
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-tabBar", this->sizes.height_tabBar, result
        );
    //position
    //scale

    //resource

    //������ǩ��
    this->tabBar = std::make_unique<TabComponent>();
    this->addAndMakeVisible(this->tabBar.get());
}

UIComponent::~UIComponent()
{
}

void UIComponent::paint (juce::Graphics& g)
{
}

void UIComponent::resized()
{
    //��ȡ��Ļ�������
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);

    //������ǩ����С
    this->tabBar->setBounds(
        0, 0,
        this->getWidth(), screenSize.getHeight() * this->sizes.height_tabBar
    );
}

void UIComponent::setCaller(const juce::String& caller)
{
    this->caller = caller;
}
