#include "TabList.h"
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

	//初始化工程状态获取函数
	this->getPtrFunc = jmadf::GetInterface<int, vocalshaper::Project*&>(
		"VocalSharp.VocalShaper.ProjectHub", "GetPtr"
		);
	this->getCurrentFunc = jmadf::GetInterface<int&>(
		"VocalSharp.VocalShaper.ProjectHub", "GetCurrent"
		);
	this->getSizeFunc = jmadf::GetInterface<int&>(
		"VocalSharp.VocalShaper.ProjectHub", "GetSize"
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

    //size
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-tabBorder", this->sizes.width_tabBorder, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-tabMarginRight", this->sizes.width_tabMarginRight, result
        );
    //position
    //scale
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-closeButton", this->scales.height_closeButton, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "icon_mainMenuButton", this->scales.icon_mainMenuButton, result
        );

    //resource
	juce::String iconCloseButtonFile, iconAddButtonFile, iconMoreButtonFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "icon-closeButton", iconCloseButtonFile, result
        );
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-addButton", iconAddButtonFile, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetString",
		"main", "resource", "icon-moreButton", iconMoreButtonFile, result
		);

	//建立字体标准label
	this->lbDefaultFont = std::make_unique<juce::Label>();
	this->addChildComponent(this->lbDefaultFont.get());
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

bool TabList::wannaClose()
{
	int size = 0;
	this->getSizeFunc(size);
	while (size > 0) {
		int current = -1;
		this->getCurrentFunc(current);
		if (!this->checkThenClose(current)) {
			//如取消
			this->refreshCompCache();
			return false;
		}
		this->getSizeFunc(size);
	}
	return true;
}

void TabList::resized()
{
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

	//绘制标签页
	int totalWidth = 0;
	for (int i = 0; i < this->tabShow.size(); i++) {
		//获取引用
		auto& item = this->tabShow.getReference(i);
		auto ptrItem = item.second;

		//大小计算
		int itemWidth = item.first;
		int strWidth = itemWidth - 3 * tabBorderWidth - closeButtonHeight;

		juce::Rectangle<int> tabRect(
			totalWidth, 0,
			itemWidth, this->getHeight()
		);
		juce::Rectangle<int> textRect(
			totalWidth + tabBorderWidth, 0,
			strWidth, this->getHeight()
		);

		//绘背景
		if (i == this->currentIndex) {
			g.setColour(this->colors.background_tabList_highlight);
		}
		else {
			if (i == this->hoverIndex) {
				g.setColour(this->colors.background_tabList.contrasting(0.05f));
			}
			else {
				g.setColour(this->colors.background_tabList);
			}
		}
		g.fillRect(tabRect);

		//绘文本
		if (i == this->currentIndex) {
			g.setColour(this->colors.text_tabList_highlight);
		}
		else {
			g.setColour(this->colors.text_tabList);
		}
		g.drawFittedText(
			ptrItem->getName(), textRect,
			juce::Justification::centred,
			1
		);

		//宽度计数
		totalWidth += itemWidth;
	}
}

void TabList::mouseMove(const juce::MouseEvent& event)
{

}

void TabList::mouseDown(const juce::MouseEvent& event)
{

}

void TabList::mouseEnter(const juce::MouseEvent& event)
{

}

void TabList::mouseExit(const juce::MouseEvent& event)
{

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
			//是
			jmadf::CallInterface<int>(
				"VocalSharp.VocalShaper.ProjectHub", "SaveProj",
				index
				);
			break;
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
	return true;
}

void TabList::refreshCompCache()
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

	//计算各项目位置
	int totalWidth = 0;
	bool foldFlag = false;

	int totalSize = 0;
	this->getSizeFunc(totalSize);

	for (int i = 0; i < totalSize; i++) {
		//获取当前指针
		vocalshaper::Project* ptrItem = nullptr;
		this->getPtrFunc(i, ptrItem);

		if (!foldFlag) {
			//计算tab宽度
			int strWidth = font.getStringWidth(ptrItem->getName());
			int itemWidth = strWidth + tabBorderWidth * 3 + closeButtonHeight;
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

	this->getCurrentFunc(this->currentIndex);

	this->repaint();
}
