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

	return true;
}

void VocalSharp_VocalShaper_LookAndFeelFactory::destory()
{
	this->factory = nullptr;
	jmadf::CallInterface<void>("WuChang.JMADF.LookAndFeelConfigs", "Close");
}