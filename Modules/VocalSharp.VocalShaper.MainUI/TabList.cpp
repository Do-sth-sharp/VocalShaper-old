﻿#include "TabList.h"
#include <libJModule.h>

TabList::TabList()
	:Component("Tab List Component")
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
	{
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
	}

	//初始化工程状态获取函数
	this->getPtrFunc = jmadf::GetInterface<int, vocalshaper::ProjectProxy*&>(
		"VocalSharp.VocalShaper.ProjectHub", "GetPtr"
		);
	this->getCurrentFunc = jmadf::GetInterface<int&>(
		"VocalSharp.VocalShaper.ProjectHub", "GetCurrent"
		);
	this->getSizeFunc = jmadf::GetInterface<int&>(
		"VocalSharp.VocalShaper.ProjectHub", "GetSize"
		);

	//以下获取命令管理器
	jmadf::CallInterface<juce::ApplicationCommandManager*&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandManager",
		this->commandManager
		);

	//以下获取命令ID
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Close Project", this->closeProjCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Close Other Project", this->closeOtherProjCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Close All Project", this->closeAllProjCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Show Start Menu", this->showStartMenuCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Save Project", this->saveProjCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Save All Project", this->saveAllProjCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Copy Path", this->copyPathCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Copy Name", this->copyNameCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Copy Full Url", this->copyFullUrlCommandID
		);
	jmadf::CallInterface<const juce::String&, int&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
		"Open Path In System", this->openPathInSystemCommandID
		);

	//以下注册命令回调
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Close Project", [this] {this->highlightCloseButtonClicked(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Close Other Project", [this] {this->closeOther(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Close All Project", [this] {
			if (this->wannaClose()) {
				this->addButtonClicked();
			}
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Show Start Menu", [this] {this->addButtonClicked(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Save Project", [this] {this->saveCurrent(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Save All Project", [this] {this->saveAll(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Copy Path", [this] {this->copyPath(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Copy Name", [this] {this->copyName(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Copy Full Url", [this] {this->copyFullUrl(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Open Path In System", [this] {this->openPathInSystem(); }
	);

	//以下注册Flag获取函数
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Close Other Project", [this]()->int {
			int size = 0;
			jmadf::CallInterface<int&>(
				"VocalSharp.VocalShaper.ProjectHub", "GetSize",
				size);

			int flag = 0;
			if (size <= 1) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
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
		"main", "color", "icon-tabBarButton", this->colors.icon_tabBarButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-tabBarButton", this->colors.background_tabBarButton, result
		);
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-tabList", this->colors.text_tabList, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-tabList", this->colors.background_tabList, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-tabList-highlight", this->colors.text_tabList_highlight, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-tabList-highlight", this->colors.background_tabList_highlight, result
        );
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "split", this->colors.split, result
		);

    //size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-tab-max", this->sizes.width_tab_max, result
		);
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-tabBorder", this->sizes.width_tabBorder, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-tabMarginRight", this->sizes.width_tabMarginRight, result
        );
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-splitLine", this->sizes.width_splitLine, result
		);
    //position
    //scale
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-closeButton", this->scales.height_closeButton, result
        );
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-splitLine", this->scales.height_splitLine, result
		);

    //resource
	juce::String iconCloseFile, iconAddFile, iconMoreFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "icon-closeButton", iconCloseFile, result
        );
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-addButton", iconAddFile, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-moreButton", iconMoreFile, result
		);

	//建立字体标准label
	this->lbDefaultFont = std::make_unique<juce::Label>();
	this->addChildComponent(this->lbDefaultFont.get());

	//加载关闭按钮图标
	size_t iconCloseSize = 0;
	void* iconClosePtr = nullptr;
	juce::String iconClosePath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
		.getParentDirectory().getFullPathName() + iconCloseFile;

	jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
		"WuChang.JMADF.DynamicRC",
		"GetRC",
		iconClosePath, std::pair<size_t&, void*&>(iconCloseSize, iconClosePtr)
		);
	if (iconClosePtr) {
		juce::String iconCloseStr((char*)iconClosePtr, iconCloseSize);
		auto ptrXml = juce::XmlDocument::parse(iconCloseStr);
		if (ptrXml) {
			this->iconClose = juce::Drawable::createFromSVG(*ptrXml);
			this->iconCloseHighlight = juce::Drawable::createFromSVG(*ptrXml);
			if (this->iconClose) {
				this->iconClose->replaceColour(
					juce::Colours::black, this->colors.text_tabList
				);
			}
			if (this->iconCloseHighlight) {
				this->iconCloseHighlight->replaceColour(
					juce::Colours::black, this->colors.text_tabList_highlight
				);
			}
		}
	}

	//加载添加按钮图标
	size_t iconAddSize = 0;
	void* iconAddPtr = nullptr;
	juce::String iconAddPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
		.getParentDirectory().getFullPathName() + iconAddFile;

	jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
		"WuChang.JMADF.DynamicRC",
		"GetRC",
		iconAddPath, std::pair<size_t&, void*&>(iconAddSize, iconAddPtr)
		);
	if (iconAddPtr) {
		juce::String iconAddStr((char*)iconAddPtr, iconAddSize);
		auto ptrXml = juce::XmlDocument::parse(iconAddStr);
		if (ptrXml) {
			this->iconAdd = juce::Drawable::createFromSVG(*ptrXml);
			if (this->iconAdd) {
				this->iconAdd->replaceColour(
					juce::Colours::black, this->colors.icon_tabBarButton
				);
			}
		}
	}

	//加载更多按钮图标
	size_t iconMoreSize = 0;
	void* iconMorePtr = nullptr;
	juce::String iconMorePath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
		.getParentDirectory().getFullPathName() + iconMoreFile;

	jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
		"WuChang.JMADF.DynamicRC",
		"GetRC",
		iconMorePath, std::pair<size_t&, void*&>(iconMoreSize, iconMorePtr)
		);
	if (iconMorePtr) {
		juce::String iconMoreStr((char*)iconMorePtr, iconMoreSize);
		auto ptrXml = juce::XmlDocument::parse(iconMoreStr);
		if (ptrXml) {
			this->iconMore = juce::Drawable::createFromSVG(*ptrXml);
			if (this->iconMore) {
				this->iconMore->replaceColour(
					juce::Colours::black, this->colors.icon_tabBarButton
				);
			}
		}
	}

	//建立关闭按钮样式
	this->lafs.tabCloseButton = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.tabCloseButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_tabList.contrasting(0.05f)
	);
	this->lafs.tabCloseButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_tabList.contrasting(0.05f)
	);
	this->lafs.tabCloseButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	this->lafs.tabHighlightCloseButton = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.tabHighlightCloseButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_tabList_highlight
	);
	this->lafs.tabHighlightCloseButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_tabList_highlight
	);
	this->lafs.tabHighlightCloseButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//初始化关闭按钮
	this->btCloseHover = std::make_unique<juce::DrawableButton>(
		"bt_TabClose", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->btCloseHover->setImages(this->iconClose.get());
	this->btCloseHover->setLookAndFeel(this->lafs.tabCloseButton.get());
	this->btCloseHover->setWantsKeyboardFocus(false);
	this->btCloseHover->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->btCloseHover->onClick = [this] {
		this->closeButtonClicked();
	};
	this->addChildComponent(this->btCloseHover.get());

	this->btCloseCurrent = std::make_unique<juce::DrawableButton>(
		"bt_TabCloseHighlight", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->btCloseCurrent->setImages(this->iconCloseHighlight.get());
	this->btCloseCurrent->setLookAndFeel(this->lafs.tabHighlightCloseButton.get());
	this->btCloseCurrent->setWantsKeyboardFocus(false);
	this->btCloseCurrent->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->btCloseCurrent->setCommandToTrigger(this->commandManager, this->closeProjCommandID, false);
	this->addChildComponent(this->btCloseCurrent.get());

	//以下构建主菜单按钮样式
	this->lafs.tabBarButton = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.tabBarButton->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_tabBarButton
	);
	this->lafs.tabBarButton->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_tabBarButton
	);
	this->lafs.tabBarButton->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//初始化添加按钮
	this->btAdd = std::make_unique<juce::DrawableButton>(
		"bt_TabAdd", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->btAdd->setImages(this->iconAdd.get());
	this->btAdd->setLookAndFeel(this->lafs.tabBarButton.get());
	this->btAdd->setWantsKeyboardFocus(false);
	this->btAdd->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->btAdd->setCommandToTrigger(this->commandManager, this->showStartMenuCommandID, false);
	this->addAndMakeVisible(this->btAdd.get());

	//初始化更多按钮
	this->btMore = std::make_unique<juce::DrawableButton>(
		"bt_TabMore", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
	this->btMore->setImages(this->iconMore.get());
	this->btMore->setLookAndFeel(this->lafs.tabBarButton.get());
	this->btMore->setWantsKeyboardFocus(false);
	this->btMore->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->btMore->onClick = [this] {
		this->moreButtonClicked();
	};
	this->addChildComponent(this->btMore.get());

	//侦听事件变化
	jmadf::CallInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules",
		[this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
		{this->listenActions(action, type); });

	//侦听保存
	jmadf::CallInterface<const vocalshaper::ProjectProxy::SaveCallbackFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddSaveCallback",
		[this](const vocalshaper::ProjectProxy* project)
		{this->listenSaved(project); });
}

bool TabList::newProj(const juce::String& name, const juce::String& path)
{
	bool result = false;
	jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "NewProject",
		name, path, result
		);
	this->refreshCompCache();
	return result;
}

bool TabList::copyProj(const juce::String& name, const juce::String& path,
	const juce::String& nameSrc, const juce::String& pathSrc)
{
	bool result = false;
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "CopyProject",
		name, path, nameSrc, pathSrc, result
		);
	this->refreshCompCache();
	return result;
}

bool TabList::openProj(const juce::String& name, const juce::String& path)
{
	bool result = false;
	jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "OpenProject",
		name, path, result
		);
	this->refreshCompCache();
	return result;
}

bool TabList::isOpenedProj(const juce::String& name, const juce::String& path)
{
	bool result = false;
	jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "IsOpened",
		name, path, result
		);
	return result;
}

bool TabList::isSavedProj(const juce::String& name, const juce::String& path)
{
	bool result = false;
	jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "IsSaved",
		name, path, result
		);
	return result;
}

bool TabList::wannaClose()
{
	int size = 0;
	this->getSizeFunc(size);
	while (size > 0) {
		int current = -1;
		this->getCurrentFunc(current);
		if (!this->checkThenClose(current)) {
			//如取消
			return false;
		}
		this->getSizeFunc(size);
	}
	return true;
}

void TabList::saveCurrent()
{
	this->save(this->currentIndex);
}

void TabList::saveAll()
{
	int size = 0;
	this->getSizeFunc(size);
	for (int i = 0; i < size; i++) {
		this->save(i);
	}
}

void TabList::closeOther()
{
	int size = 0;
	this->getSizeFunc(size);
	int current = this->currentIndex;
	for (int i = size - 1; i >= 0; i--) {
		if (i > current || i < current) {
			if (!this->checkThenClose(i)) {
				break;
			}
		}
		if (i < current) {
			current--;
		}
	}
}

void TabList::copyPath()
{
	if (this->currentIndex >= 0 && this->currentIndex < this->tabShow.size()) {
		//获取引用
		auto& item = this->tabShow.getReference(this->currentIndex);
		auto ptrItem = item.second;

		//复制路径
		juce::SystemClipboard::copyTextToClipboard(ptrItem->getPath());
	}
}

void TabList::copyName()
{
	if (this->currentIndex >= 0 && this->currentIndex < this->tabShow.size()) {
		//获取引用
		auto& item = this->tabShow.getReference(this->currentIndex);
		auto ptrItem = item.second;

		//复制名称
		juce::SystemClipboard::copyTextToClipboard(ptrItem->getName());
	}
}

void TabList::copyFullUrl()
{
	if (this->currentIndex >= 0 && this->currentIndex < this->tabShow.size()) {
		//获取引用
		auto& item = this->tabShow.getReference(this->currentIndex);
		auto ptrItem = item.second;

		//获取路径
		juce::String url = juce::File(ptrItem->getPath() + "/" 
			+ ptrItem->getName() + this->projectExtension).getFullPathName();

		//复制名称
		juce::SystemClipboard::copyTextToClipboard(url);
	}
}

void TabList::openPathInSystem()
{
	if (this->currentIndex >= 0 && this->currentIndex < this->tabShow.size()) {
		//获取引用
		auto& item = this->tabShow.getReference(this->currentIndex);
		auto ptrItem = item.second;

		//打开路径
		juce::Process::openDocument(ptrItem->getPath(), juce::String());
	}
}

void TabList::resized()
{
	//高度缓存
	bool heightChanged = false;
	if (this->heightTemp != this->getHeight()) {
		this->heightTemp = this->getHeight();
		heightChanged = true;
	}

	//更新标签
	this->refreshCompCache();
}

void TabList::paint(juce::Graphics& g)
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//通过label作为字体标准
	juce::Font font =
		juce::LookAndFeel::getDefaultLookAndFeel().getLabelFont(*(this->lbDefaultFont.get()));
	g.setFont(font);

	//填充背景
	g.fillAll(this->colors.background);

	//计算控件大小
	int tabBorderWidth = this->sizes.width_tabBorder * screenSize.getWidth();
	int closeButtonHeight = this->getHeight() * this->scales.height_closeButton;
	int splitWidth = this->sizes.width_splitLine * screenSize.getWidth();
	int splitHeight = this->scales.height_splitLine * this->getHeight();
	int splitPosY = this->getHeight() / 2 - splitHeight / 2;
	float roundSize = 6.0f;

	//绘制标签页
	int totalWidth = 0;
	for (int i = 0; i < this->tabShow.size(); i++) {
		//获取引用
		auto& item = this->tabShow.getReference(i);
		auto ptrItem = item.second;

		//大小计算
		int itemWidth = item.first;
		int strWidth = itemWidth - 3 * tabBorderWidth - closeButtonHeight;

		juce::Rectangle<float> tabRect(
			totalWidth, 0,
			itemWidth, this->getHeight()
		);
		juce::Rectangle<float> tabRectTop(
			totalWidth, 0,
			itemWidth, this->getHeight() / 2
		);
		juce::Rectangle<float> tabRectBottom(
			totalWidth, this->getHeight() / 2,
			itemWidth, this->getHeight() / 2
		);
		juce::Rectangle<int> textRect(
			totalWidth + tabBorderWidth, 0,
			strWidth, this->getHeight()
		);

		//绘背景
		if (i == this->currentIndex) {
			//当前标签已激活
			g.setColour(this->colors.background_tabList);
			g.fillRect(tabRectTop);
			g.setColour(this->colors.background_tabList_highlight);
			g.fillRect(tabRectBottom);
			g.fillRoundedRectangle(tabRect, roundSize);
		}
		else {
			g.setColour(this->colors.background_tabList);
			g.fillRect(tabRect);
			if (i == this->hoverIndex) {
				g.setColour(this->colors.background_tabList.contrasting(0.05f));
				g.fillRoundedRectangle(tabRect, roundSize);
			}
		}

		//检查是否保存
		bool isSaved = true;
		{
			juce::ScopedReadLock projLocker(ptrItem->getLock());
			isSaved = ptrItem->getSaved();
		}

		//绘文本
		if (i == this->currentIndex) {
			g.setColour(this->colors.text_tabList_highlight);
		}
		else {
			g.setColour(this->colors.text_tabList);
		}
		g.drawFittedText(
			(isSaved ? "" : "*") + ptrItem->getName(), textRect,
			juce::Justification::centred,
			1, 1.0f
		);

		//绘分割线
		if (
			i != 0 &&
			i != this->currentIndex &&
			(i - 1) != this->currentIndex
			) {
			g.setColour(this->colors.split);
			g.fillRect(totalWidth - splitWidth / 2, splitPosY, splitWidth, splitHeight);
		}

		//宽度计数
		totalWidth += itemWidth;
	}

	//绘末尾分割线
	/*if (
		this->tabShow.size() == 0 ||
		(this->tabShow.size() - 1) != this->currentIndex
		) {
		g.setColour(this->colors.split);
		g.fillRect(totalWidth - splitWidth / 2, splitPosY, splitWidth, splitHeight);
	}*/
}

void TabList::mouseMove(const juce::MouseEvent& event)
{
	//获取鼠标位置
	int posX = event.getPosition().getX();

	//获取控件位置
	auto screenBounds = this->getScreenBounds();

	//计算鼠标所在标签页
	int totalWidth = 0;
	for (int i = 0; i < this->tabShow.size(); i++) {
		//获取引用
		auto& item = this->tabShow.getReference(i);
		int itemWidth = item.first;
		auto itemPtr = item.second;

		//如果当前标签页命中
		if (totalWidth <= posX && totalWidth + itemWidth > posX) {
			if (this->hoverIndex != i) {
				//修改鼠标指针
				this->setMouseCursor(juce::MouseCursor::PointingHandCursor);

				//显示弹出提示
				this->setTooltip(itemPtr->getPath());
				
				//修改覆盖状态
				this->hoverIndex = i;
				this->refreshComp();
				this->repaint();
			}
			return;
		}

		//宽度计数
		totalWidth += itemWidth;
	}

	//未命中任何标签页则修改鼠标指针
	this->setMouseCursor(juce::MouseCursor::NormalCursor);

	//关闭弹出提示
	this->setTooltip(juce::String());
}

void TabList::mouseDown(const juce::MouseEvent& event)
{
	//获取鼠标位置
	int posX = event.getPosition().getX();

	//计算鼠标所在标签页
	int totalWidth = 0;
	for (int i = 0; i < this->tabShow.size(); i++) {
		//获取引用
		auto& item = this->tabShow.getReference(i);
		int itemWidth = item.first;

		//如果当前标签页命中
		if (totalWidth <= posX && totalWidth + itemWidth > posX) {
			//设置当前标签页
			if (this->currentIndex != i) {
				jmadf::CallInterface<int>(
					"VocalSharp.VocalShaper.ProjectHub", "SetCurrent",
					i
					);
				this->refreshCompCache();
			}

			//显示右键菜单
			if (event.mods.isPopupMenu()) {
				this->showPopupMenu();
			}
			return;
		}

		//宽度计数
		totalWidth += itemWidth;
	}
}

void TabList::mouseEnter(const juce::MouseEvent& event)
{
	this->mouseMove(event);
}

void TabList::mouseExit(const juce::MouseEvent& event)
{
	//获取鼠标位置
	int posX = event.getPosition().getX();
	int posY = event.getPosition().getY();

	//计算总区域
	int totalWidth = 0;
	for (int i = 0; i < this->tabShow.size(); i++) {
		//获取引用
		auto& item = this->tabShow.getReference(i);
		int itemWidth = item.first;

		//宽度计数
		totalWidth += itemWidth;
	}

	//判断是否真正离开
	if (!(posX > 0 && posX < totalWidth && posY > 0 && posY < this->getHeight())) {
		//修改鼠标指针
		this->setMouseCursor(juce::MouseCursor::NormalCursor);

		//修改覆盖状态
		this->hoverIndex = -1;
		this->refreshComp();
		this->repaint();
	}
}

void TabList::closeButtonClicked()
{
	if (this->checkThenClose(this->hoverIndex)) {
		//修改鼠标指针
		this->setMouseCursor(juce::MouseCursor::NormalCursor);

		//刷新标签
		this->refreshCompCache();
	}
}

void TabList::highlightCloseButtonClicked()
{
	if (this->checkThenClose(this->currentIndex)) {
		//修改鼠标指针
		this->setMouseCursor(juce::MouseCursor::NormalCursor);

		//刷新标签
		this->refreshCompCache();
	}
}

void TabList::addButtonClicked()
{
	//唤起开始菜单
	jmadf::CallInterface<void>(
		this->caller, "ShowStartMenu");
}

void TabList::moreButtonClicked()
{
	//构建项目菜单
	juce::PopupMenu listMenu;
	for (int i = 0; i < this->tabHide.size(); i++) {
		listMenu.addItem(i + 1, this->tabHide.getUnchecked(i)->getName());
	}

	//展示菜单，获取结果
	int result = listMenu.showAt(this->btMore.get());

	//判断结果
	if (result > 0) {
		jmadf::CallInterface<int>(
			"VocalSharp.VocalShaper.ProjectHub", "SetCurrentAndToFront",
			this->tabShow.size() + (result - 1)
			);
		this->refreshCompCache();
	}
}

void TabList::setCaller(const juce::String& caller)
{
	this->caller = caller;
}

void TabList::listenActions(const vocalshaper::actions::ActionBase& /*action*/, vocalshaper::actions::ActionBase::UndoType /*type*/)
{
	//获取消息管理器
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}

	//刷新
	messageManager->callAsync([this] {this->refreshCompCache(); });
}

void TabList::listenSaved(const vocalshaper::ProjectProxy* /*proj*/)
{
	//获取消息管理器
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}

	//刷新
	messageManager->callAsync([this] {this->refreshCompCache(); });
}

bool TabList::checkThenClose(int index)
{
	//检查是否已保存
	bool isSaved = false;
	jmadf::CallInterface<int, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "CheckForClose",
		index, isSaved
		);

	//如果未保存
	if (!isSaved) {
		int result = juce::AlertWindow::showYesNoCancelBox(
			juce::MessageBoxIconType::QuestionIcon,
			this->tr("bt_CloseProject"), this->tr("tip_SaveBeforeClose"),
			this->tr("bt_Yes"), this->tr("bt_No"), this->tr("bt_Cancel")
		);
		switch (result)
		{
		case 1:
		{
			//是
			bool saveOK = false;
			jmadf::CallInterface<int, bool&>(
				"VocalSharp.VocalShaper.ProjectHub", "SaveProj",
				index, saveOK
				);
			if (!saveOK) {
				juce::AlertWindow::showMessageBox(
					juce::MessageBoxIconType::WarningIcon,
					this->tr("tip_SaveProject"), this->tr("tip_SaveProjectError"),
					this->tr("bt_OK")
				);
				return false;
			}
			break;
		}
		case 2:
			//否
			break;
		default:
			//取消
			return false;
		}
	}

	jmadf::CallInterface<int>(
		"VocalSharp.VocalShaper.ProjectHub", "CloseProj",
		index
		);
	this->refreshCompCache();
	return true;
}

bool TabList::save(int index)
{
	//检查是否已保存
	bool isSaved = false;
	jmadf::CallInterface<int, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "CheckForClose",
		index, isSaved
		);

	//如果未保存
	if (!isSaved) {
		bool saveOK = false;
		jmadf::CallInterface<int, bool&>(
			"VocalSharp.VocalShaper.ProjectHub", "SaveProj",
			index, saveOK
			);
		if (!saveOK) {
			juce::AlertWindow::showMessageBox(
				juce::MessageBoxIconType::WarningIcon,
				this->tr("tip_SaveProject"), this->tr("tip_SaveProjectError"),
				this->tr("bt_OK")
			);
			return false;
		}
	}
	return true;
}

void TabList::refreshCompCache(bool loopFlag)
{
	//进行清理
	this->tabShow.clearQuick();
	this->tabHide.clearQuick();
	this->hoverIndex = -1;
	this->currentIndex = -1;

	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//通过label作为字体标准
	juce::Font font =
		juce::LookAndFeel::getDefaultLookAndFeel().getLabelFont(*(this->lbDefaultFont.get()));

	//计算控件大小
	int rightMarginWidth = this->sizes.width_tabMarginRight * screenSize.getWidth();
	int availableWidth = this->getWidth() - this->getHeight() * 2 - rightMarginWidth;
	int tabBorderWidth = this->sizes.width_tabBorder * screenSize.getWidth();
	int closeButtonHeight = this->getHeight() * this->scales.height_closeButton;

	int tabLimitWidth = this->sizes.width_tab_max * screenSize.getWidth();

	//计算各项目位置
	int totalWidth = 0;
	bool foldFlag = false;

	int totalSize = 0;
	this->getSizeFunc(totalSize);

	if (totalSize == 0) {
		//没有标签时，唤起开始菜单
		jmadf::CallInterface<void>(
			this->caller, "ShowStartMenu");

		//进行重绘
		this->refreshComp();
		this->repaint();
		return;
	}

	for (int i = 0; i < totalSize; i++) {
		//获取当前指针
		vocalshaper::ProjectProxy* ptrItem = nullptr;
		this->getPtrFunc(i, ptrItem);

		if (!foldFlag) {
			//检查是否保存
			bool isSaved = true;
			{
				juce::ScopedReadLock projLocker(ptrItem->getLock());
				isSaved = ptrItem->getSaved();
			}

			//计算tab宽度
			int strWidth = font.getStringWidth((isSaved ? "" : "*") + ptrItem->getName());
			int itemWidth = strWidth + tabBorderWidth * 3 + closeButtonHeight;

			//限制tab宽度
			itemWidth = std::min(itemWidth, tabLimitWidth);

			if (totalWidth + itemWidth > availableWidth) {
				//当前已超限
				this->tabHide.add(ptrItem);
				foldFlag = true;
			}
			else {
				//当前未超限
				this->tabShow.add(std::make_pair(itemWidth, ptrItem));
				totalWidth += itemWidth;
			}
		}
		else {
			//已到达折叠区，无需计算宽度
			this->tabHide.add(ptrItem);
		}
	}

	//获取当前激活的工程
	this->getCurrentFunc(this->currentIndex);

	//当未折叠区域有标签页时，确保激活的标签页在未折叠区域
	if (this->currentIndex >= this->tabShow.size() && !loopFlag) {
		jmadf::CallInterface<int>(
			"VocalSharp.VocalShaper.ProjectHub", "SetCurrentAndToFront",
			this->currentIndex
			);
		this->refreshCompCache(true);
		return;
	}

	//进行重绘
	this->refreshComp();
	this->repaint();
}

void TabList::refreshComp()
{
	//进行清理
	this->btCloseCurrent->setVisible(false);
	this->btCloseHover->setVisible(false);

	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int rightMarginWidth = this->sizes.width_tabMarginRight * screenSize.getWidth();
	int tabBorderWidth = this->sizes.width_tabBorder * screenSize.getWidth();
	int closeButtonHeight = this->getHeight() * this->scales.height_closeButton;

	//遍历并排布关闭按钮
	int totalWidth = 0;
	for (int i = 0; i < this->tabShow.size(); i++) {
		//获取引用
		auto& item = this->tabShow.getReference(i);
		int itemWidth = item.first;

		//计算关闭按钮位置
		int buttonPosX = totalWidth + itemWidth - tabBorderWidth - closeButtonHeight;

		if (i == this->currentIndex) {
			//激活的选项卡
			this->btCloseCurrent->setBounds(
				buttonPosX, this->getHeight() / 2 - closeButtonHeight / 2,
				closeButtonHeight, closeButtonHeight
			);
			this->btCloseCurrent->setVisible(true);
		}
		else if (i == this->hoverIndex) {
			//鼠标覆盖的选项卡
			this->btCloseHover->setBounds(
				buttonPosX, this->getHeight() / 2 - closeButtonHeight / 2,
				closeButtonHeight, closeButtonHeight
			);
			this->btCloseHover->setVisible(true);
		}

		//宽度计数
		totalWidth += itemWidth;
	}

	//放置添加按钮
	this->btAdd->setBounds(
		totalWidth, 0,
		this->getHeight(), this->getHeight()
	);

	//放置更多按钮
	this->btMore->setBounds(
		this->getWidth() - rightMarginWidth - this->getHeight(), 0,
		this->getHeight(), this->getHeight()
	);
	this->btMore->setVisible(this->tabHide.size() > 0);
}

void TabList::showPopupMenu()
{
	juce::PopupMenu menu;

	menu.addCommandItem(this->commandManager, this->saveProjCommandID);
	menu.addCommandItem(this->commandManager, this->saveAllProjCommandID);
	menu.addCommandItem(this->commandManager, this->closeProjCommandID);
	menu.addCommandItem(this->commandManager, this->closeOtherProjCommandID);
	menu.addCommandItem(this->commandManager, this->closeAllProjCommandID);
	menu.addSeparator();
	menu.addCommandItem(this->commandManager, this->copyNameCommandID);
	menu.addCommandItem(this->commandManager, this->copyPathCommandID);
	menu.addCommandItem(this->commandManager, this->copyFullUrlCommandID);
	menu.addCommandItem(this->commandManager, this->openPathInSystemCommandID);
	menu.addSeparator();
	menu.addCommandItem(this->commandManager, this->showStartMenuCommandID);

	menu.show();
}
