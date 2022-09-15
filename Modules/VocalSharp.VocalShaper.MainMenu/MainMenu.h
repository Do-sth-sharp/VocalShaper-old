#pragma once
#include <JuceHeader.h>

class MainMenu final
{
	MainMenu() = delete;

public:
	static juce::PopupMenu create();
	static void active(int id);

private:
	static std::function<const juce::String(const juce::String&)> tr;

	static juce::PopupMenu createFileMenu();
	static juce::PopupMenu createEditMenu();
	static juce::PopupMenu createViewMenu();
	static juce::PopupMenu createModifyMenu();
	static juce::PopupMenu createProjectMenu();
	static juce::PopupMenu createTransportMenu();
	static juce::PopupMenu createAutomaticMenu();
	static juce::PopupMenu createConfigMenu();
	static juce::PopupMenu createMiscMenu();
};