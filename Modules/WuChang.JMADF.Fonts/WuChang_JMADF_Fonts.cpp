#include "WuChang_JMADF_Fonts.h"

bool WuChang_JMADF_Fonts::init()
{
	if (!jmadf::LoadModule("WuChang.JMADF.DynamicRC")) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, std::pair<size_t&, void*&>>(
			"WuChang.JMADF.DynamicRC", "GetRC") ||
		!jmadf::CheckInterface<const juce::String&>(
			"WuChang.JMADF.DynamicRC", "ReleaseRC")
		) {
		jmadf::RaiseException("@WuChang.JMADF.DynamicRC:Bad Interfaces!");
		return false;
	}
	this->fonts = std::make_unique<Fonts>();
	
	jmadf::RegisterInterface<const juce::String&, juce::Typeface::Ptr&, bool&>(
		"GetFont",
		[this](const juce::String& caller, const juce::String& fontName, juce::Typeface::Ptr& ptr, bool& result) {
			result = this->fonts->getFont(caller, fontName, ptr);
		}
	);
	jmadf::RegisterInterface<juce::Typeface::Ptr&, bool&>(
		"GetDefault",
		[this](const juce::String& caller, juce::Typeface::Ptr& ptr, bool& result) {
			result = this->fonts->getDefault(caller, ptr);
		}
	);

	jmadf::RegisterInterface<const juce::String&>(
		"SetDefaultFont",
		[this](const juce::String&, const juce::String& lang) {
			this->fonts->setDefaultFont(lang);
		}
	);
	jmadf::RegisterInterface<juce::String&>(
		"GetDefaultFont",
		[this](const juce::String&, juce::String& lang) {
			lang = this->fonts->getDefaultFont();
		}
	);

	jmadf::RegisterInterface<void>(
		"ReleaseAll",
		[this](const juce::String&)
		{
			this->fonts->releaseAll();
		}
	);

	jmadf::SetUnloadHook(
		[this](const juce::String& caller)
		{
			this->fonts->close(caller);
		}
	);
	return true;
}

void WuChang_JMADF_Fonts::destory()
{
	this->fonts = nullptr;
}