#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ClipBoard final
{
public:
	ClipBoard() = default;
	~ClipBoard() = default;

	using ArrayType = vocalshaper::SPSList;
	using StackType = juce::OwnedArray<ArrayType>;

	ArrayType* add();
	const ArrayType* get();
	const ArrayType* get(int index);
	juce::StringArray getList();
	int size();
	void clean();

	void acceptCopyAndDelete(
		const juce::String& caller,
		const vocalshaper::EditorBase* editor);
	void acceptPaste(
		const juce::String& caller,
		const vocalshaper::EditorBase* editor);
	void unacceptCopyAndDelete(
		const vocalshaper::EditorBase* editor);
	void unacceptPaste(
		const vocalshaper::EditorBase* editor);
	void release(const juce::String& caller);

	bool couldCopyAndDelete();
	bool couldPaste();
	void sendCopy();
	void sendDelete();
	void sendSelectAll();
	void sendPaste();
	void sendPasteWithIndex(int index);

	bool couldCopyToSystem();
	bool couldPasteFromSystem();
	void sendCopyToSystem();
	void sendPasteFromSystem();

private:
	StackType list;
	juce::ReadWriteLock lock;

	struct CopyAndDeleteMethod final
	{
		juce::String caller;
		vocalshaper::EditorBase* editor = nullptr;
	};
	struct PasteMethod final
	{
		juce::String caller;
		vocalshaper::EditorBase* editor = nullptr;
	};

	CopyAndDeleteMethod cadMethod;
	PasteMethod pMethod;
	juce::ReadWriteLock funcLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipBoard)
};