#include "TopLevelEditorBase.h"
#include <libJModule.h>

TopLevelEditorBase::TopLevelEditorBase()
	: EditorBase()
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
