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

    //以下获取命令ID
    this->initCommandID();

    //以下注册命令回调
    this->initCommandFunction();

    //以下注册Flag获取函数
    this->initCommandFlagHook();

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
            this->getWidth(), this->getHeight() - this->tabBar->getHeight() - this->playBar->getHeight() - 1
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

bool UIComponent::isOpenedProj(const juce::String& name, const juce::String& path)
{
	return this->tabBar->isOpenedProj(name, path);
}

bool UIComponent::isSavedProj(const juce::String& name, const juce::String& path)
{
	return this->tabBar->isSavedProj(name, path);
}

bool UIComponent::wannaClose()
{
    return this->tabBar->wannaClose();
}

vocalshaper::AdsorbState UIComponent::getAdsorb()
{
    return this->adsorbState;
}

void UIComponent::setAdsorb(vocalshaper::AdsorbState state)
{
    this->adsorbState = state;
    this->playBar->setAdsorb(state);
	jmadf::CallInterface<vocalshaper::AdsorbState>(
		"VocalSharp.VocalShaper.MainEditor", "SetAdsorb", state);
}

vocalshaper::GridState UIComponent::getGrid()
{
    return this->gridState;
}

void UIComponent::setGrid(vocalshaper::GridState state)
{
    this->gridState = state;
    this->playBar->setGrid(state);
	jmadf::CallInterface<vocalshaper::GridState>(
		"VocalSharp.VocalShaper.MainEditor", "SetGrid", state);
}

void UIComponent::initCommandID()
{

}

void UIComponent::initCommandFunction()
{
    //Adsorb
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1/2 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1_2Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1/4 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1_4Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1/6 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1_6Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1/8 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1_8Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1/12 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1_12Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1/16 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1_16Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1/24 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1_24Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb 1/32 Beat", [this] {this->setAdsorb(vocalshaper::AdsorbState::Adsorb1_32Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Adsorb Off", [this] {this->setAdsorb(vocalshaper::AdsorbState::AdsorbOff); }
    );
    //Grid
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1/2 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1_2Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1/4 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1_4Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1/6 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1_6Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1/8 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1_8Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1/12 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1_12Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1/16 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1_16Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1/24 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1_24Beat); }
    );
    jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
        "VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
        "Grid 1/32 Beat", [this] {this->setGrid(vocalshaper::GridState::Grid1_32Beat); }
    );
}

void UIComponent::initCommandFlagHook()
{
	//Adsorb
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/2 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1_2Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/4 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1_4Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/6 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1_6Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/8 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1_8Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/12 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1_12Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/16 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1_16Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/24 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1_24Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb 1/32 Beat", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::Adsorb1_32Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Adsorb Off", [this]()->int {
			int flag = 0;
			if (this->getAdsorb() == vocalshaper::AdsorbState::AdsorbOff) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	//Grid
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/2 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1_2Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/4 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1_4Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/6 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1_6Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/8 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1_8Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/12 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1_12Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/16 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1_16Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/24 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1_24Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Grid 1/32 Beat", [this]()->int {
			int flag = 0;
			if (this->getGrid() == vocalshaper::GridState::Grid1_32Beat) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
}
