#pragma once
#include <libVocalShaper.h>
#include <JuceHeader.h>

class KPSSpliter final : public vocalshaper::SpliterBase
{
public:
	KPSSpliter() = default;
	~KPSSpliter() override = default;

	const SentenceList getSentence(const vocalshaper::Track* track) const override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KPSSpliter)
};