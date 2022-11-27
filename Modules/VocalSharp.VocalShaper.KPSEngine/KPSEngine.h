#pragma once
#include <libVocalShaper.h>
#include <JuceHeader.h>
#include "KPSRenderer.h"
#include "KPSSpliter.h"
#include "KPSPhonerator.h"

class KPSEngine final : public vocalshaper::EngineBase
{
public:
	KPSEngine();
	~KPSEngine() override = default;

	const EngineFlags getEngineFlags() const override;
	const RenderMode getRenderMode() const override;
	const vocalshaper::ParamInfoList getParamList() const override;
	vocalshaper::RendererBase const* getRenderer() const override;
	vocalshaper::SpliterBase const* getSpliter() const override;
	vocalshaper::PhoneratorBase const* getPhonerator() const override;

private:
	std::unique_ptr<KPSRenderer> renderer = nullptr;
	std::unique_ptr<KPSSpliter> spliter = nullptr;
	std::unique_ptr<KPSPhonerator> phonerator = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KPSEngine)
};