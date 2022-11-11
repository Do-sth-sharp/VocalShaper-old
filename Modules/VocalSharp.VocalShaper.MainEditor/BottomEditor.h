#pragma once
#include <JuceHeader.h>
#include "TopLevelEditorBase.h"
#include "StatusBar.h"

class BottomEditor final : public TopLevelEditorBase
{
public:
	BottomEditor();
	~BottomEditor() override = default;

	void resized() override;
	void paint(juce::Graphics& g) override;

public:
	void setAdditionEditorIsShow(bool additionEditorIsShow);
	bool isAdditionEditorShow();

	enum class EditorType
	{
		NoteEditor,
		MixEditor,
		ScriptEditor
	};

	void setEditorType(EditorType type);
	EditorType getEditorType();

	void selectNoteEditorPlugin();
	void selectNoteEditionAdditionPlugin();
	int switchTrack();

private:
	struct Size final : public TopLevelEditorBase::Size
	{
		double height_statusBar;
	}sizes;//控件大小

	void initCommandID();
	void initCommandFunction();
	void initCommandFlagHook();

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

	std::unique_ptr<StatusBar> statusBar;

	//临时
	bool additionEditorIsShow = false;
	EditorType editorType = EditorType::NoteEditor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomEditor)
};