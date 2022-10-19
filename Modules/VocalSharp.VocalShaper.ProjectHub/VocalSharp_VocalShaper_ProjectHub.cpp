#include "VocalSharp_VocalShaper_ProjectHub.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_ProjectHub::VocalSharp_VocalShaper_ProjectHub()
	:Module()
{
	
}

VocalSharp_VocalShaper_ProjectHub::~VocalSharp_VocalShaper_ProjectHub()
{
	
}

bool VocalSharp_VocalShaper_ProjectHub::init()
{
	if (!(
		jmadf::LoadModule("VocalSharp.VocalShaper.ProjectIO") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.CallbackReactor")
		)) {
		return false;
	}
	if (
		!jmadf::CheckInterface<vocalshaper::ProjectProxy*, bool&>(
			"VocalSharp.VocalShaper.ProjectIO", "ReadProject") ||
		!jmadf::CheckInterface<vocalshaper::ProjectProxy*, bool&>(
			"VocalSharp.VocalShaper.ProjectIO", "WriteProject")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.ProjectIO:Bad Interfaces!");
		return false;
	}
	if (
		!jmadf::CheckInterface<vocalshaper::ProjectProxy*>(
			"VocalSharp.VocalShaper.CallbackReactor", "Process")
		) {
		jmadf::RaiseException("@VocalSharp.VocalShaper.CallbackReactor:Bad Interfaces!");
		return false;
	}

	this->projects = std::make_unique<ProjectHub>();

	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"NewProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = this->projects->newProj(name, path);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, const juce::String&, const juce::String&, bool&>(
		"CopyProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path,
			const juce::String& nameSrc, const juce::String& pathSrc, bool& result)
		{
			result = this->projects->copyProj(name, path, nameSrc, pathSrc);
		}
	);
	jmadf::RegisterInterface<const juce::String&, const juce::String&, bool&>(
		"OpenProject",
		[this](const juce::String& caller,
			const juce::String& name, const juce::String& path, bool& result)
		{
			result = this->projects->openProj(name, path);
		}
	);

	jmadf::RegisterInterface<int>(
		"SetCurrent",
		[this](const juce::String&, int index) {
			this->projects->setCurrent(index);
		}
	);
	jmadf::RegisterInterface<int>(
		"SetCurrentAndToFront",
		[this](const juce::String&, int index) {
			this->projects->setCurrentAndToFront(index);
		}
	);
	jmadf::RegisterInterface<int>(
		"CloseProj",
		[this](const juce::String&, int index) {
			this->projects->close(index);
		}
	);
	jmadf::RegisterInterface<int, bool&>(
		"CheckForClose",
		[this](const juce::String&, int index, bool& result) {
			result = this->projects->checkForClose(index);
		}
	);
	jmadf::RegisterInterface<int, vocalshaper::ProjectProxy*&>(
		"GetPtr",
		[this](const juce::String&, int index, vocalshaper::ProjectProxy*& result) {
			result = this->projects->get(index);
		}
	);
	jmadf::RegisterInterface<int&>(
		"GetCurrent",
		[this](const juce::String&, int& result) {
			result = this->projects->getCurrent();
		}
	);
	jmadf::RegisterInterface<int&>(
		"GetSize",
		[this](const juce::String&, int& result) {
			result = this->projects->getSize();
		}
	);
	jmadf::RegisterInterface<int, bool&>(
		"SaveProj",
		[this](const juce::String&, int index, bool& result) {
			result = this->projects->save(index);
		}
	);
	jmadf::RegisterInterface<const ProjectHub::ChangeNoticeFunction&>(
		"AddNotice",
		[this](const juce::String& caller, const ProjectHub::ChangeNoticeFunction& func) {
			this->projects->addNotice(caller, func);
		}
	);
	jmadf::RegisterInterface<void>(
		"Release",
		[this](const juce::String& caller) {
			this->projects->release(caller);
		}
	);
	return true;
}

void VocalSharp_VocalShaper_ProjectHub::destory()
{
	this->projects = nullptr;
}