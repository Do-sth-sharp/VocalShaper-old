#include "TopEditor.h"
#include <libJModule.h>

TopEditor::TopEditor()
	: TopLevelEditorBase()
{
	//以下获取命令ID
	this->initCommandID();

	//以下注册命令回调
	this->initCommandFunction();

	//以下注册Flag获取函数
	this->initCommandFlagHook();

	//以下获取编辑器
	jmadf::CallInterface<vocalshaper::EditorBase*&>(
		"VocalSharp.VocalShaper.TrackEditor", "GetPtr", this->topEditor);
	this->addAndMakeVisible(this->topEditor);
}

void TopEditor::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	if (this->topEditor) {
		this->topEditor->projectChanged(ptr);
	}
}

void TopEditor::trackChanged(int trackID)
{
	if (this->topEditor) {
		this->topEditor->trackChanged(trackID);
	}
}

void TopEditor::setEditMode(bool editMode)
{
	if (this->topEditor) {
		this->topEditor->setEditMode(editMode);
	}
}

void TopEditor::setToolID(uint8_t toolID)
{
	if (this->topEditor) {
		this->topEditor->setToolID(toolID);
	}
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
	if (this->topEditor) {
		this->topEditor->setBounds(0, 0, this->getWidth() - this->toolBar->getWidth(), this->getHeight());
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
