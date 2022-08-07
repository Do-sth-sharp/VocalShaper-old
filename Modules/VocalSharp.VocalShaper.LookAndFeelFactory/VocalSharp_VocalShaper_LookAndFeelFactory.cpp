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
	return true;
}

void VocalSharp_VocalShaper_LookAndFeelFactory::destory()
{
	this->factory = nullptr;
	jmadf::CallInterface<void>("WuChang.JMADF.LookAndFeelConfigs", "Close");
}