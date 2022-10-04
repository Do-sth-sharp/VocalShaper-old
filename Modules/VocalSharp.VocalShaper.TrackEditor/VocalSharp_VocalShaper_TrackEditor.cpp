#include "VocalSharp_VocalShaper_TrackEditor.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_TrackEditor::VocalSharp_VocalShaper_TrackEditor()
	:Module()
{
	
}

VocalSharp_VocalShaper_TrackEditor::~VocalSharp_VocalShaper_TrackEditor()
{
	
}

bool VocalSharp_VocalShaper_TrackEditor::init()
{
	this->editor = std::make_unique<TrackEditor>();

	if (!this->editor) {
		jmadf::RaiseException("Can't alloc memory space for editor!");
		return false;
	}

	vocalshaper::EditorBase* ptrEditor = this->editor.get();
	jmadf::RegisterInterface<vocalshaper::EditorBase*&>(
		"GetPtr",
		[ptrEditor](const juce::String&, vocalshaper::EditorBase*& ptr) {
			ptr = ptrEditor;
		}
	);

	return true;
}

void VocalSharp_VocalShaper_TrackEditor::destory()
{
	this->editor = nullptr;
}