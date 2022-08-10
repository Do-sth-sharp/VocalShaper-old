#include "VocalSharp_VocalShaper_ProjectHistory.h"

VocalSharp_VocalShaper_ProjectHistory::VocalSharp_VocalShaper_ProjectHistory()
	:Module()
{
	
}

VocalSharp_VocalShaper_ProjectHistory::~VocalSharp_VocalShaper_ProjectHistory()
{
	
}

bool VocalSharp_VocalShaper_ProjectHistory::init()
{
	if (!jmadf::LoadModule("WuChang.JMADF.Configs")) {
		return false;
	}

	this->hm = std::make_unique<HistoryManager>();
	jmadf::RegisterInterface<int&>(
		"GetSize", [this](const juce::String&, int& size) {
			size = this->hm->getSize();
		}
	);
	jmadf::RegisterInterface<int, juce::String&>(
		"GetName", [this](const juce::String&, int index, juce::String& value) {
			value = this->hm->getName(index);
		}
	);
	jmadf::RegisterInterface<int, juce::String&>(
		"GetPath", [this](const juce::String&, int index, juce::String& value) {
			value = this->hm->getPath(index);
		}
	);
	jmadf::RegisterInterface<int, juce::String&>(
		"GetTime", [this](const juce::String&, int index, juce::String& value) {
			value = this->hm->getTime(index);
		}
	);

	jmadf::RegisterInterface<const juce::String&, const juce::String&>(
		"Add", [this](const juce::String&, const juce::String& name, const juce::String& path) {
			this->hm->add(name, path);
		}
	);
	jmadf::RegisterInterface<int>(
		"Open", [this](const juce::String&, int index) {
			this->hm->open(index);
		}
	);
	jmadf::RegisterInterface<int>(
		"Remove", [this](const juce::String&, int index) {
			this->hm->remove(index);
		}
	);

	jmadf::RegisterInterface<const juce::String&>(
		"SetFilter", [this](const juce::String&, const juce::String& filter) {
			this->hm->setFilter(filter);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_ProjectHistory::destory()
{
	this->hm = nullptr;
	jmadf::CallInterface<void>("WuChang.JMADF.Configs", "Close");
}