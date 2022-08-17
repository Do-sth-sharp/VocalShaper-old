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
}

bool TabList::newProj(const juce::String& name, const juce::String& path)
{
	bool result = false;
	jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "NewProject",
		name, path, result
		);
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
	return result;
}

bool TabList::openProj(const juce::String& name, const juce::String& path)
{
	bool result = false;
	jmadf::CallInterface<const juce::String&, const juce::String&, bool&>(
		"VocalSharp.VocalShaper.ProjectHub", "OpenProject",
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

void TabList::resized()
{

}

void TabList::paint(juce::Graphics& g)
{

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
