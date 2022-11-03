#include "LabelEditor.h"
#include <libJModule.h>

LabelEditor::LabelEditor()
	: Component("Label Editor")
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

	//初始化标签类型下拉框
	this->comboBox = std::make_unique<juce::ComboBox>("Label Type");
	this->comboBox->addItemList({ "LUA", "INI", "XML", "JSON" }, 1);
	this->comboBox->setSelectedItemIndex(1);
	this->comboBox->onChange = [this] {this->checkLabelData(); };
	this->addAndMakeVisible(this->comboBox.get());

	//TODO 初始化编辑器与数据模型
}

void LabelEditor::setProject(vocalshaper::ProjectProxy* project)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = project;
	this->refreshLabel();
}

void LabelEditor::setLabelIndex(int index)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->index = index;
	this->refreshLabel();
}

void LabelEditor::resized()
{

}

void LabelEditor::refreshLabel()
{
	//TODO 加载标签数据并检查
}

void LabelEditor::checkLabelData()
{
	//TODO 检查标签数据
}
