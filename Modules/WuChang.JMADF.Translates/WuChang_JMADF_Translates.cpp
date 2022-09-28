#include "WuChang_JMADF_Translates.h"

bool WuChang_JMADF_Translates::init()
{
	this->translates = std::make_unique<Translates>();
	jmadf::RegisterInterface<std::function<const juce::String(const juce::String&)>&>(
		"GetFunc",
		[this](const juce::String& caller, std::function<const juce::String(const juce::String&)>& func)
		{
			juce::var* list = nullptr;
			this->translates->getReference(caller, this->translates->getCurrentLang(), list);
			func = [this, list, caller](const juce::String& str)->const juce::String {
				return this->translates->trFast(caller, list, str);
			};
		}
	);
	jmadf::RegisterInterface<const juce::String&>(
		"SetCurrentLang",
		[this](const juce::String&, const juce::String& lang) {
			this->translates->setCurrentLang(lang);
		}
	);
	jmadf::RegisterInterface<juce::String&>(
		"GetCurrentLang",
		[this](const juce::String&, juce::String& lang) {
			lang = this->translates->getCurrentLang();
		}
	);
	jmadf::RegisterInterface<const juce::StringArray&>(
		"SetDefaultLang",
		[this](const juce::String&, const juce::StringArray& langList) {
			this->translates->setDefaultLang(langList);
		}
	);

	jmadf::RegisterInterface<void>(
		"ReleaseAll",
		[this](const juce::String&)
		{
			this->translates->releaseAll();
		}
	);
	jmadf::RegisterInterface<const juce::String&>(
		"Release",
		[this](const juce::String&, const juce::String& moduleName)
		{
			this->translates->release(moduleName);
		}
	);
	jmadf::RegisterInterface<void>(
		"Close",
		[this](const juce::String& caller)
		{
			this->translates->release(caller);
		}
	);
	return true;
}

void WuChang_JMADF_Translates::destory()
{
	this->translates = nullptr;
}