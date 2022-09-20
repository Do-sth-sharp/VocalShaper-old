#include "VocalSharp_VocalShaper_LookAndFeelFactory.h"

VocalSharp_VocalShaper_LookAndFeelFactory::VocalSharp_VocalShaper_LookAndFeelFactory()
	:Module()
{
	
}

VocalSharp_VocalShaper_LookAndFeelFactory::~VocalSharp_VocalShaper_LookAndFeelFactory()
{
	
}

bool VocalSharp_VocalShaper_LookAndFeelFactory::init()
{
	if (!jmadf::LoadModule("WuChang.JMADF.LookAndFeelConfigs")) {
		return false;
	}
	this->factory = std::make_unique<LAFFactory>();

	jmadf::RegisterInterface<int, juce::LookAndFeel*&>(
		"GetStartMenuButtonLAF",
		[this](const juce::String&, int fontHeight, juce::LookAndFeel*& laf) {
			laf = this->factory->getStartMenuButtonLAF(fontHeight);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetStartMenuTextEditorLAF",
		[this](const juce::String&, juce::LookAndFeel*& laf) {
			laf = this->factory->getStartMenuTextEditorLAF();
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetStartMenuListBoxLAF",
		[this](const juce::String&, juce::LookAndFeel*& laf) {
			laf = this->factory->getStartMenuListBoxLAF();
		}
	);

	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetMainMenuButtonLAF",
		[this](const juce::String&, juce::LookAndFeel*& laf) {
			laf = this->factory->getMainMenuButtonLAF();
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetTabCloseButtonLAF",
		[this](const juce::String&, juce::LookAndFeel*& laf) {
			laf = this->factory->getTabCloseButtonLAF();
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetPlayButtonLAF",
		[this](const juce::String&, juce::LookAndFeel*& laf) {
			laf = this->factory->getPlayButtonLAF();
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetToolButtonLAF",
		[this](const juce::String&, juce::LookAndFeel*& laf) {
			laf = this->factory->getToolButtonLAF();
		}
	);

	return true;
}

void VocalSharp_VocalShaper_LookAndFeelFactory::destory()
{
	this->factory = nullptr;
	jmadf::CallInterface<void>("WuChang.JMADF.LookAndFeelConfigs", "Close");
}