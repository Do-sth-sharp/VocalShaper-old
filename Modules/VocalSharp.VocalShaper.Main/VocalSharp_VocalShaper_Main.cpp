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
	//��ȡ����
	if (!(jmadf::LoadModule("WuChang.JMADF.Configs") &&
		jmadf::LoadModule("WuChang.JMADF.GlobalConfigs") &&
		jmadf::LoadModule("VocalSharp.VocalShaper.ProjectHub")
		)) {
		return false;
	}
	juce::var* config = nullptr;
	bool ok = false;
	jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
		"WuChang.JMADF.GlobalConfigs", "GetReference",
		"config", config, ok
		);
	juce::String lang;
	juce::String font;
	if (ok && (config != nullptr)) {
		if ((*config)["Language"].isString()) {
			lang = (*config)["Language"].toString();
		}
		if ((*config)["Font"].isString()) {
			font = (*config)["Font"].toString();
		}
	}

	//��������
	jmadf::LoadModule("WuChang.JMADF.Translates");
	jmadf::CallInterface<const juce::String&>(
		"WuChang.JMADF.Translates", "SetCurrentLang",
		lang
		);

	//��������
	if (!jmadf::LoadModule("WuChang.JMADF.Fonts")) {
		return false;
	}
	jmadf::CallInterface<const juce::String&>(
		"WuChang.JMADF.Fonts", "SetDefaultFont",
		font
		);

	bool fontOK = false;
	juce::Typeface::Ptr ptrFont = nullptr;
	jmadf::CallInterface<juce::Typeface::Ptr&, bool&>(
		"WuChang.JMADF.Fonts", "GetDefault",
		ptrFont, fontOK
		);
	if (fontOK) {
		juce::LookAndFeel::getDefaultLookAndFeel()
			.setDefaultSansSerifTypeface(ptrFont);
	}

	//��ʼ����ʽ����
	if (!jmadf::LoadModule("VocalSharp.VocalShaper.LookAndFeelFactory")) {
		return false;
	}

	//�������ʼ��
	const char* mainWindowName = "VocalSharp.VocalShaper.MainWindow";
	if (!jmadf::LoadModule(mainWindowName)) {
		jmadf::RaiseException("@ERROR " + juce::String(mainWindowName));
		return false;
	}
	return true;
}

void VocalSharp_VocalShaper_Main::destory()
{
	jmadf::CallInterface<void>("WuChang.JMADF.Fonts", "Close");
	jmadf::CallInterface<void>("WuChang.JMADF.Translates", "Close");
	jmadf::CallInterface<void>("WuChang.JMADF.Configs", "Close");
	jmadf::CallInterface<void>("WuChang.JMADF.GlobalConfigs", "Close");
}