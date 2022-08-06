#include "Fonts.h"
#include <libJModule.h>

bool Fonts::getFont(const juce::String& caller, const juce::String& fontName, juce::Typeface::Ptr& ptr)
{
	Types* type = nullptr;
	if (!this->load(caller, fontName, type)) {
		return false;
	}
	ptr = type->font;
	return true;
}

bool Fonts::getDefault(const juce::String& caller, juce::Typeface::Ptr& ptr)
{
	return this->getFont(caller, this->getDefaultFont(), ptr);
}

void Fonts::releaseAll()
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	this->list.clear();
}

void Fonts::close(const juce::String& caller)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	for (auto it = this->list.begin(); it != this->list.end();) {
		auto mit = (*it).second.modules.find(caller);
		if (mit != (*it).second.modules.end()) {
			(*it).second.modules.erase(mit);
		}
		if ((*it).second.modules.empty()) {
			it = this->list.erase(it);
			continue;
		}
		it++;
	}
}

bool Fonts::load(const juce::String& caller, const juce::String& fontName, Types*& type)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	auto it = this->list.find(fontName);
	if (it != this->list.end()) {
		type = &((*it).second);
		type->modules.insert(caller);
		return true;
	}

	size_t fontSize = 0;
	void* fontPtr = nullptr;
	juce::String fontPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
		.getParentDirectory().getFullPathName() + "/types/" + fontName + ".ttf";

	jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
		"WuChang.JMADF.DynamicRC",
		"GetRC",
		fontPath, std::pair<size_t&, void*&>(fontSize, fontPtr)
		);
	if (fontPtr == nullptr) {
		return false;
	}

	auto& typeRef = this->list[fontName];
	typeRef = {
		.font = juce::Typeface::createSystemTypefaceFor(fontPtr, fontSize),
		.modules = {caller}
	};

	jmadf::CallInterface<const juce::String&>(
		"WuChang.JMADF.DynamicRC", "ReleaseRC",
		fontPath
		);

	type = &typeRef;
	return true;
}

void Fonts::setDefaultFont(const juce::String& font)
{
	this->defaultFont = font;
}

const juce::String Fonts::getDefaultFont()
{
	return this->defaultFont;
}
