#include "KPSPhonerator.h"

const vocalshaper::PhonemeInfoListWithDeviation KPSPhonerator::getPhoneme(
	const vocalshaper::Track* track, int noteIndex, const std::tuple<int, int> currentSentence) const
{
	return vocalshaper::PhonemeInfoListWithDeviation();
}

const vocalshaper::PhonemeInfoList KPSPhonerator::getAllPhonemesForSinger(
	const juce::String& singer, const juce::String& style) const
{
	return vocalshaper::PhonemeInfoList();
}
