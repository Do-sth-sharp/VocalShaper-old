#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class LabelEditor final : public juce::Component
{
public:
	LabelEditor();
	~LabelEditor() override = default;

	void setProject(vocalshaper::ProjectProxy* project);
	void setLabelIndex(int index);

	void resized() override;

private:
	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	std::function<const juce::String(const juce::String&)> tr;

	std::unique_ptr<juce::ComboBox> comboBox = nullptr;
	std::unique_ptr<juce::CodeEditorComponent> labelEditor = nullptr;
	std::unique_ptr<juce::CodeDocument> document = nullptr;
	std::unique_ptr<juce::Label> result = nullptr;
	std::unique_ptr<juce::TextButton> okButton = nullptr;

	vocalshaper::ProjectProxy* project = nullptr;
	int index = -1;
	juce::ReadWriteLock projectLock;

	void refreshLabel();
	void checkLabelData();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabelEditor)
};