#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TopEditor final : public juce::Component
{
public:
	TopEditor();
	~TopEditor() override = default;

	void projectChanged(const vocalshaper::ProjectProxy* ptr);

	bool isActive();
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> getCopy();
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> getCut();
	bool wannaDelete();
	bool wannaCopy();
	bool wannaSelectAll();
	bool wannaPaste(juce::OwnedArray<vocalshaper::SerializableProjectStructure> list);
	bool wannaPaste(const juce::StringArray& list);
	int showClipBoard(const juce::StringArray& list);

	void paint(juce::Graphics& g) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopEditor);
};