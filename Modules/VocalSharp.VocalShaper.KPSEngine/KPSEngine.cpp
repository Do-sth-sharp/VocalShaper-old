#include "KPSEngine.h"

KPSEngine::KPSEngine()
	: EngineBase()
{
	this->renderer = std::make_unique<KPSRenderer>();
	this->spliter = std::make_unique<KPSSpliter>();
	this->phonerator = std::make_unique<KPSPhonerator>();
}

const vocalshaper::EngineBase::EngineFlags KPSEngine::getEngineFlags() const
{
	return vocalshaper::EngineBase::EngineFlags{
		.renderBasePitch = false,
		.exportPitch = true,
		.exportDynamic = true,
		.exportSpectrogrum = true,
		.exportFormantrogrum = false
	};
}

const vocalshaper::EngineBase::RenderMode KPSEngine::getRenderMode() const
{
	return vocalshaper::EngineBase::RenderMode::Both;
}

const vocalshaper::ParamInfoList KPSEngine::getParamList() const
{
	return vocalshaper::ParamInfoList{
		vocalshaper::ParamInfo{vocalshaper::ParamInfo::ParamType::Dynamic, "PIT", "Pitch", -12., 12., 0., true},
		vocalshaper::ParamInfo{vocalshaper::ParamInfo::ParamType::Dynamic, "DYN", "Dynamic", 0., 1., 0.5, false},
		vocalshaper::ParamInfo{vocalshaper::ParamInfo::ParamType::Static, "OPE", "Strength", 0., 1., 0.5, false}
	};
}

vocalshaper::RendererBase const* KPSEngine::getRenderer() const
{
	return this->renderer.get();
}

vocalshaper::SpliterBase const* KPSEngine::getSpliter() const
{
	return this->spliter.get();
}

vocalshaper::PhoneratorBase const* KPSEngine::getPhonerator() const
{
	return this->phonerator.get();
}