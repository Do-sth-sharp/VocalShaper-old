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

	jmadf::RegisterInterface<ClipBoard::ArrayType&&>(
		"CopyAndCut", [this](const juce::String&, ClipBoard::ArrayType&& array) {
			this->clipBoard->copyAndCut(std::move(array));
		}
	);
	jmadf::RegisterInterface<ClipBoard::ArrayType&>(
		"Paste", [this](const juce::String&, ClipBoard::ArrayType& array) {
			array = this->clipBoard->paste();
		}
	);
	jmadf::RegisterInterface<ClipBoard::ArrayType&, int>(
		"PasteItem", [this](const juce::String&,
			ClipBoard::ArrayType& array, int index) {
				array = this->clipBoard->paste(index);
		}
	);
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

	return true;
}

void VocalSharp_VocalShaper_ClipBoard::destory()
{
	this->clipBoard = nullptr;
}