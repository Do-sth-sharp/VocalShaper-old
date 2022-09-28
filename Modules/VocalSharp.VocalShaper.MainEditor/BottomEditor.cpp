#include "BottomEditor.h"
#include <libJModule.h>

BottomEditor::BottomEditor()
	: TopLevelEditorBase()
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//以下获取界面属性
	bool result = false;
	//color
	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-toolBar", this->sizes.width_toolBar, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-statusBar", this->sizes.height_statusBar, result
		);

	//position
	//scale
	//resource

	//创建底栏
	this->statusBar = std::make_unique<StatusBar>();
	this->addAndMakeVisible(this->statusBar.get());
}

void BottomEditor::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	//TODO
}

void BottomEditor::setEditMode(bool editMode)
{
	//TODO
}

void BottomEditor::setToolID(uint8_t toolID)
{
	//TODO
}

bool BottomEditor::isActive()
{
	//TODO 判定活动区
	return this->isVisible() && this->hasKeyboardFocus(true);
}

juce::OwnedArray<vocalshaper::SerializableProjectStructure> BottomEditor::getCopy()
{
	//TODO
	return juce::OwnedArray<vocalshaper::SerializableProjectStructure>();
}

juce::OwnedArray<vocalshaper::SerializableProjectStructure> BottomEditor::getCut()
{
	//TODO
	return juce::OwnedArray<vocalshaper::SerializableProjectStructure>();
}

bool BottomEditor::wannaDelete()
{
	//TODO
	return false;
}

bool BottomEditor::wannaCopy()
{
	//TODO
	return false;
}

bool BottomEditor::wannaSelectAll()
{
	//TODO
	return false;
}

bool BottomEditor::wannaPaste(juce::OwnedArray<vocalshaper::SerializableProjectStructure> list)
{
	//TODO
	return false;
}

bool BottomEditor::wannaPaste(const juce::StringArray& list)
{
	//TODO
	return false;
}

int BottomEditor::showClipBoard(const juce::StringArray& list)
{
	//TODO
	return -1;
}

void BottomEditor::resized()
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算并调整底栏高度
	int height_statusBar = this->sizes.height_statusBar * screenSize.getHeight();
	int width_toolBar = this->sizes.width_toolBar * screenSize.getWidth();
	this->statusBar->setBounds(
		0, this->getHeight() - height_statusBar,
		this->getWidth() - width_toolBar - 1, height_statusBar
	);

	//调整工具栏位置
	this->TopLevelEditorBase::resized();
}

void BottomEditor::paint(juce::Graphics& g)
{
	//g.fillAll(juce::Colours::blueviolet);
}
