#include "VocalSharp_VocalShaper_ClipBoard.h"
#include <libJModule.h>
#include <libVocalShaper.h>

VocalSharp_VocalShaper_ClipBoard::VocalSharp_VocalShaper_ClipBoard()
	:Module()
{
	
}

VocalSharp_VocalShaper_ClipBoard::~VocalSharp_VocalShaper_ClipBoard()
{
	
}

bool VocalSharp_VocalShaper_ClipBoard::init()
{
	this->clipBoard = std::make_unique<ClipBoard>();
	return true;
}

void VocalSharp_VocalShaper_ClipBoard::destory()
{
	this->clipBoard = nullptr;
}