#include "TopEditor.h"
#include <libJModule.h>

TopEditor::TopEditor()
	: TopLevelEditorBase()
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

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
		"main", "size", "width-toolBar", this->sizes.width_toolBar, result
		);

	//position
	//scale
	//resource

	//以下获取编辑器
	jmadf::CallInterface<vocalshaper::EditorBase*&>(
		"VocalSharp.VocalShaper.TrackEditor", "GetPtr", this->topEditor);
	this->addChildEditorAndMakeVisible(this->topEditor);
}

bool TopEditor::isActive()
{
	if (this->topEditor) {
		return this->topEditor->isActive();
	}
	return false;
}

juce::OwnedArray<vocalshaper::SerializableProjectStructure> TopEditor::getCopy()
{
	if (this->topEditor) {
		return this->topEditor->getCopy();
	}
	return juce::OwnedArray<vocalshaper::SerializableProjectStructure>();
}

juce::OwnedArray<vocalshaper::SerializableProjectStructure> TopEditor::getCut()
{
	if (this->topEditor) {
		return this->topEditor->getCut();
	}
	return juce::OwnedArray<vocalshaper::SerializableProjectStructure>();
}

bool TopEditor::wannaDelete()
{
	if (this->topEditor) {
		return this->topEditor->wannaDelete();
	}
	return false;
}

bool TopEditor::wannaCopy()
{
	if (this->topEditor) {
		return this->topEditor->wannaCopy();
	}
	return false;
}

bool TopEditor::wannaSelectAll()
{
	if (this->topEditor) {
		return this->topEditor->wannaSelectAll();
	}
	return false;
}

bool TopEditor::wannaPaste(juce::OwnedArray<vocalshaper::SerializableProjectStructure> list)
{
	if (this->topEditor) {
		return this->topEditor->wannaPaste(std::move(list));
	}
	return false;
}

bool TopEditor::wannaPaste(const juce::StringArray& list)
{
	if (this->topEditor) {
		return this->topEditor->wannaPaste(list);
	}
	return false;
}

int TopEditor::showClipBoard(const juce::StringArray& list)
{
	if (this->topEditor) {
		return this->topEditor->showClipBoard(list);
	}
	return -1;
}

void TopEditor::resized()
{
	//获取屏幕相关属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算并调整底栏高度
	int width_toolBar = this->sizes.width_toolBar * screenSize.getWidth();

	if (this->topEditor) {
		this->topEditor->setBounds(0, 0,
			this->getWidth() - width_toolBar - 1, this->getHeight());
	}
	this->TopLevelEditorBase::resized();
}

void TopEditor::initCommandID()
{

}

void TopEditor::initCommandFunction()
{
	
}

void TopEditor::initCommandFlagHook()
{

}
