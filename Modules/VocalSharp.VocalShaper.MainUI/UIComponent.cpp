#include "UIComponent.h"
#include <libJModule.h>

UIComponent::UIComponent()
    : Component("Main UI Component")
{
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
            );

}

UIComponent::~UIComponent()
{
}

void UIComponent::paint (juce::Graphics& g)
{
}

void UIComponent::resized()
{
}

void UIComponent::setCaller(const juce::String& caller)
{
    this->caller = caller;
}
