#include "TopEditor.h"

TopEditor::TopEditor()
	: Component("Top Editor")
{

}

void TopEditor::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	//TODO
}

bool TopEditor::isActive()
{
	//TODO 判定活动区
	return this->hasKeyboardFocus(true);
}

juce::OwnedArray<vocalshaper::SerializableProjectStructure> TopEditor::getCopy()
{
	//TODO
	return juce::OwnedArray<vocalshaper::SerializableProjectStructure>();
}

juce::OwnedArray<vocalshaper::SerializableProjectStructure> TopEditor::getCut()
{
	//TODO
	return juce::OwnedArray<vocalshaper::SerializableProjectStructure>();
}

bool TopEditor::wannaDelete()
{
	//TODO
	return false;
}

bool TopEditor::wannaCopy()
{
	//TODO
	return false;
}

bool TopEditor::wannaSelectAll()
{
	//TODO
	return false;
}

bool TopEditor::wannaPaste(juce::OwnedArray<vocalshaper::SerializableProjectStructure> list)
{
	//TODO
	return false;
}

bool TopEditor::wannaPaste(const juce::StringArray& list)
{
	//TODO
	return false;
}

int TopEditor::showClipBoard(const juce::StringArray& list)
{
	//TODO
	return -1;
}

void TopEditor::paint(juce::Graphics& g)
{
	//g.fillAll(juce::Colours::darkseagreen);
}
