#include "VocalSharp_VocalShaper_Main.h"

VocalSharp_VocalShaper_Main::VocalSharp_VocalShaper_Main()
	:Module()
{
	
}

VocalSharp_VocalShaper_Main::~VocalSharp_VocalShaper_Main()
{
	
}

bool VocalSharp_VocalShaper_Main::init()
{
	//获取语言设置
	if (!jmadf::LoadModule("WuChang.JMADF.Configs")) {
		return false;
	}
	juce::var* config = nullptr;
	bool ok = false;
	jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
		"WuChang.JMADF.Configs", "GetReference",
		"config", config, ok
		);
	juce::String lang;
	if (ok && (config != nullptr)) {
		if ((*config)["Language"].isString()) {
			lang = (*config)["Language"].toString();
		}
	}

	//设置语言
	jmadf::LoadModule("WuChang.JMADF.Translates");
	jmadf::CallInterface<const juce::String&>(
		"WuChang.JMADF.Translates", "SetCurrentLang",
		lang
		);

	const char* mainWindowName = "VocalSharp.VocalShaper.MainWindow";
	if (!jmadf::LoadModule(mainWindowName)) {
		jmadf::RaiseException("@ERROR " + juce::String(mainWindowName));
		return false;
	}
	return true;
}

void VocalSharp_VocalShaper_Main::destory()
{
	jmadf::CallInterface<void>("WuChang.JMADF.Translates", "Close");
	jmadf::CallInterface<void>("WuChang.JMADF.Configs", "Close");
}