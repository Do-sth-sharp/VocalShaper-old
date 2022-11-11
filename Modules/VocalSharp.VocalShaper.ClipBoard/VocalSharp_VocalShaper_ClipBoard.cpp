#include "VocalSharp_VocalShaper_ClipBoard.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_ClipBoard::VocalSharp_VocalShaper_ClipBoard()
	:Module()
{
	
}

VocalSharp_VocalShaper_ClipBoard::~VocalSharp_VocalShaper_ClipBoard()
{
	
}

bool VocalSharp_VocalShaper_ClipBoard::init()
{
	this->clipBoard = std::make_unique<ClipBoard>();

	jmadf::RegisterInterface<juce::StringArray&>(
		"GetList", [this](const juce::String&, juce::StringArray& result) {
			result = this->clipBoard->getList();
		}
	);
	jmadf::RegisterInterface<int&>(
		"GetSize", [this](const juce::String&, int& result) {
			result = this->clipBoard->size();
		}
	);
	jmadf::RegisterInterface<void>(
		"Clean", [this](const juce::String&) {
			this->clipBoard->clean();
		}
	);

	jmadf::RegisterInterface<const vocalshaper::EditorBase*>(
			"AcceptCopyAndDelete", [this](const juce::String& caller,
				const vocalshaper::EditorBase* editor) {
					this->clipBoard->acceptCopyAndDelete(caller, editor);
		}
	);
	jmadf::RegisterInterface<const vocalshaper::EditorBase*>(
			"AcceptPaste", [this](const juce::String& caller,
				const vocalshaper::EditorBase* editor) {
					this->clipBoard->acceptPaste(caller, editor);
			}
	);
	jmadf::RegisterInterface<const vocalshaper::EditorBase*>(
		"UnacceptCopyAndDelete", [this](const juce::String&,
			const vocalshaper::EditorBase* editor) {
				this->clipBoard->unacceptCopyAndDelete(editor);
			}
	);
	jmadf::RegisterInterface<const vocalshaper::EditorBase*>(
		"UnacceptPaste", [this](const juce::String&,
			const vocalshaper::EditorBase* editor) {
				this->clipBoard->unacceptPaste(editor);
			}
	);

	jmadf::RegisterInterface<bool&>(
		"CouldCopyAndDelete", [this](const juce::String&, bool& result) {
			result = this->clipBoard->couldCopyAndDelete();
		}
	);
	jmadf::RegisterInterface<bool&>(
		"CouldPaste", [this](const juce::String&, bool& result) {
			result = this->clipBoard->couldPaste();
		}
	);

	jmadf::RegisterInterface<void>(
		"SendCopy", [this](const juce::String&) {
			this->clipBoard->sendCopy();
		}
	);
	jmadf::RegisterInterface<void>(
		"SendDelete", [this](const juce::String&) {
			this->clipBoard->sendDelete();
		}
	);
	jmadf::RegisterInterface<void>(
		"SendSelectAll", [this](const juce::String&) {
			this->clipBoard->sendSelectAll();
		}
	);
	jmadf::RegisterInterface<void>(
		"SendPaste", [this](const juce::String&) {
			this->clipBoard->sendPaste();
		}
	);
	jmadf::RegisterInterface<int>(
		"SendPasteWithIndex", [this](const juce::String&, int index) {
			this->clipBoard->sendPasteWithIndex(index);
		}
	);

	jmadf::RegisterInterface<bool&>(
		"CouldCopyToSystem", [this](const juce::String&, bool& result) {
			result = this->clipBoard->couldCopyToSystem();
		}
	);
	jmadf::RegisterInterface<bool&>(
		"CouldPasteFromSystem", [this](const juce::String&, bool& result) {
			result = this->clipBoard->couldPasteFromSystem();
		}
	);

	jmadf::RegisterInterface<void>(
		"SendCopyToSystem", [this](const juce::String&) {
			this->clipBoard->sendCopyToSystem();
		}
	);
	jmadf::RegisterInterface<void>(
		"SendPasteFromSystem", [this](const juce::String&) {
			this->clipBoard->sendPasteFromSystem();
		}
	);

	jmadf::SetUnloadHook(
		[this](const juce::String& caller) {
			this->clipBoard->release(caller);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_ClipBoard::destory()
{
	this->clipBoard = nullptr;
}