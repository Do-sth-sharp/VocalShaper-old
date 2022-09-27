#include "UIComponent.h"
#include <libJModule.h>

UIComponent::UIComponent()
    : Component("Main UI Component")
{
    //获取屏幕属性接口
    this->screenSizeFunc =
        jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
            "WuChang.JMADF.Device", "GetScreenSize"
            );

    //获取键盘监听器
    jmadf::CallInterface<juce::ApplicationCommandManager*&>(
        "VocalSharp.VocalShaper.CommandManager", "GetCommandManager",
        this->commandManager
        );
    if (this->commandManager) {
        this->addKeyListener(this->commandManager->getKeyMappings());
    }

    //以下获取界面属性
    bool result = false;
    //color

    //size
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-tabBar", this->sizes.height_tabBar, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-playBar", this->sizes.height_playBar, result
        );
    //position
    //scale

    //resource

    //建立标签栏
    this->tabBar = std::make_unique<TabComponent>();
    this->addAndMakeVisible(this->tabBar.get());

    //建立标签栏
    this->playBar = std::make_unique<PlayBar>();
    this->addAndMakeVisible(this->playBar.get());

    //获取编辑器
    jmadf::CallInterface<juce::Component*&>(
        "VocalSharp.VocalShaper.MainEditor", "GetPtr",
        this->ptrEditor);

    //建立编辑器
    if (this->ptrEditor) {
        this->addAndMakeVisible(this->ptrEditor);
    }

    //空白处获取焦点
    this->setWantsKeyboardFocus(true);
}

UIComponent::~UIComponent()
{
}

void UIComponent::paint (juce::Graphics& g)
{
}

void UIComponent::resized()
{
    //获取屏幕相关属性
    juce::Rectangle<int> screenSize;
    this->screenSizeFunc(this, screenSize);

    //调整标签栏大小
    this->tabBar->setBounds(
        0, 0,
        this->getWidth(), screenSize.getHeight() * this->sizes.height_tabBar
    );

    //调整播放控制栏大小
    this->playBar->setBounds(
        0, this->tabBar->getHeight(),
        this->getWidth(), screenSize.getHeight() * this->sizes.height_playBar
    );

    //调整编辑器大小
    if (this->ptrEditor) {
        this->ptrEditor->setBounds(
            0, this->tabBar->getHeight() + this->playBar->getHeight() + 1,
            this->getWidth(), this->getHeight() - this->tabBar->getHeight() - this->playBar->getHeight()
        );
    }
}

void UIComponent::setCaller(const juce::String& caller)
{
    this->tabBar->setCaller(caller);
}

bool UIComponent::newProj(const juce::String& name, const juce::String& path)
{
    return this->tabBar->newProj(name, path);
}

bool UIComponent::copyProj(const juce::String& name, const juce::String& path,
    const juce::String& nameSrc, const juce::String& pathSrc)
{
    return this->tabBar->copyProj(name, path, nameSrc, pathSrc);
}

bool UIComponent::openProj(const juce::String& name, const juce::String& path)
{
    return this->tabBar->openProj(name, path);
}

bool UIComponent::wannaClose()
{
    return this->tabBar->wannaClose();
}
