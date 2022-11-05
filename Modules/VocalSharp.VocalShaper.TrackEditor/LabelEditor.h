﻿#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class LabelEditor final : public juce::Component
{
public:
	LabelEditor();
	~LabelEditor() override;

	void setProject(vocalshaper::ProjectProxy* project);
	void setLabelIndex(int index);

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	struct Colors final
	{
		juce::Colour background_labelEditor;

		juce::Colour text_comboBox;
		juce::Colour background_comboBox;

		juce::Colour text_codeEditor;
		juce::Colour background_codeEditor;
		juce::Colour background_codeEditor_highlight;
		juce::Colour caret_codeEditor;

		juce::Colour thumb_codeEditor_scroller;
		juce::Colour track_codeEditor_scroller;

		juce::Colour text_labelEditorResultLabel;
		juce::Colour text_labelEditorResultLabel_error;
		juce::Colour background_labelEditorResultLabel;

		juce::Colour text_labelEditorButton;
		juce::Colour background_labelEditorButton;

		juce::Colour code_error;
		juce::Colour code_comment;
		juce::Colour code_keyword;
		juce::Colour code_operator;
		juce::Colour code_identifier;
		juce::Colour code_integer;
		juce::Colour code_float;
		juce::Colour code_string;
		juce::Colour code_bracket;
		juce::Colour code_punctuation;
		juce::Colour code_preprocessorText;
	}colors;//界面颜色
	struct Size final
	{
		double height_labelEditorMarginTop;
		double height_labelEditorMarginBottom;
		double width_labelEditorMargin;

		double width_labelEditorComboBox;
		double width_labelEditorButton;
	}sizes;//控件大小
	struct Positions final
	{
		double posY_labelEditorCodeEditor;
		double posY_labelEditorResultLabel;
		double posY_labelEditorButton;

		double posX_labelEditorComboBox;
		double posX_labelEditorButton;
	}positions;//控件位置
	struct Scale final
	{
		double height_labelEditorComboBox;
		double height_labelEditorCodeEditor;
		double height_labelEditorResultLabel;
		double height_labelEditorButton;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		juce::LookAndFeel* button;
		juce::LookAndFeel* label;
		juce::LookAndFeel* codeEditor;
		juce::LookAndFeel* comboBox;
	}lafs;//控件样式

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	std::function<const juce::String(const juce::String&)> tr;

	std::unique_ptr<juce::ComboBox> comboBox = nullptr;
	std::unique_ptr<juce::CodeEditorComponent> luaLabelEditor = nullptr;
	std::unique_ptr<juce::CodeEditorComponent> iniLabelEditor = nullptr;
	std::unique_ptr<juce::CodeEditorComponent> xmlLabelEditor = nullptr;
	std::unique_ptr<juce::CodeEditorComponent> jsonLabelEditor = nullptr;
	juce::CodeDocument document;
	std::unique_ptr<juce::CodeDocument::Listener> documentListener = nullptr;
	std::unique_ptr<juce::Label> resultLabel = nullptr;
	std::unique_ptr<juce::TextButton> okButton = nullptr;
	std::unique_ptr<juce::LuaTokeniser> luaTokeniser = nullptr;
	std::unique_ptr<juce::LuaTokeniser> iniTokeniser = nullptr;
	std::unique_ptr<juce::XmlTokeniser> xmlTokeniser = nullptr;
	std::unique_ptr<juce::CPlusPlusCodeTokeniser> jsonTokeniser = nullptr;

	vocalshaper::ProjectProxy* project = nullptr;
	int index = -1;
	double time = 0.;
	double lastTempo = 120.;
	uint8_t lastBeat = 4;
	juce::ReadWriteLock projectLock;

	void refreshLabel();
	void comboBoxChanged();
	void checkLabelData();
	void acceptAndClose();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabelEditor)
};
