#include "BottomEditor.h"

BottomEditor::BottomEditor()
	: TopLevelEditorBase()
{

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

void BottomEditor::paint(juce::Graphics& g)
{
	//g.fillAll(juce::Colours::blueviolet);
}
