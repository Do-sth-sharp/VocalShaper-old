#include "TopLevelEditorBase.h"
#include <libJModule.h>

TopLevelEditorBase::TopLevelEditorBase()
	: Component("Top Level Editor")
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//以下获取界面属性
	{
		bool result = false;
		//color
		//size
		jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
			"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
			"main", "size", "width-toolBar", this->sizes.width_toolBar, result
			);

		//position
		//scale
		//resource
	}

	//建立工具栏
	this->toolBar = std::make_unique<ToolBar>();
	this->addAndMakeVisible(this->toolBar.get());
}

void TopLevelEditorBase::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	//TODO
}

void TopLevelEditorBase::setEditMode(bool editMode)
{
	//TODO
}

void TopLevelEditorBase::setToolID(uint8_t toolID)
{
	//TODO
}

bool TopLevelEditorBase::isActive()
{
	//TODO 判定活动区
	return this->isVisible() && this->hasKeyboardFocus(true);
}

juce::OwnedArray<vocalshaper::SerializableProjectStructure> TopLevelEditorBase::getCopy()
{
	//TODO
	return juce::OwnedArray<vocalshaper::SerializableProjectStructure>();
}

juce::OwnedArray<vocalshaper::SerializableProjectStructure> TopLevelEditorBase::getCut()
{
	//TODO
	return juce::OwnedArray<vocalshaper::SerializableProjectStructure>();
}

bool TopLevelEditorBase::wannaDelete()
{
	//TODO
	return false;
}

bool TopLevelEditorBase::wannaCopy()
{
	//TODO
	return false;
}

bool TopLevelEditorBase::wannaSelectAll()
{
	//TODO
	return false;
}

bool TopLevelEditorBase::wannaPaste(juce::OwnedArray<vocalshaper::SerializableProjectStructure> list)
{
	//TODO
	return false;
}

bool TopLevelEditorBase::wannaPaste(const juce::StringArray& list)
{
	//TODO
	return false;
}

int TopLevelEditorBase::showClipBoard(const juce::StringArray& list)
{
	//TODO
	return -1;
}

void TopLevelEditorBase::resized()
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算并调整工具栏宽度
	int width_toolBar = this->sizes.width_toolBar * screenSize.getWidth();
	this->toolBar->setBounds(
		this->getWidth() - width_toolBar, 0,
		width_toolBar, this->getHeight()
	);
}
