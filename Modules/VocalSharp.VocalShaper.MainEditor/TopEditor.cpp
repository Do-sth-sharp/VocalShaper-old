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
