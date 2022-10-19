#include "VocalSharp_VocalShaper_CallbackReactor.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_CallbackReactor::VocalSharp_VocalShaper_CallbackReactor()
	:Module()
{
	
}

VocalSharp_VocalShaper_CallbackReactor::~VocalSharp_VocalShaper_CallbackReactor()
{
	
}

bool VocalSharp_VocalShaper_CallbackReactor::init()
{
	this->callbacks = std::make_unique<CallbackReactor>();
	return true;
}

void VocalSharp_VocalShaper_CallbackReactor::destory()
{
	this->callbacks = nullptr;
}