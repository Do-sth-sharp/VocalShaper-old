#pragma once
#include <libVocalShaper.h>
#include <JuceHeader.h>

class KPSPhonerator final : public vocalshaper::PhoneratorBase
{
public:
	KPSPhonerator() = default;
	~KPSPhonerator() override = default;

	const vocalshaper::PhonemeInfoListWithDeviation getPhoneme(
		const vocalshaper::Track* track, int noteIndex, const std::tuple<int, int> currentSentence) const override;

	const vocalshaper::PhonemeInfoList getAllPhonemesForSinger(
		const juce::String& singer, const juce::String& style) const override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KPSPhonerator)
};