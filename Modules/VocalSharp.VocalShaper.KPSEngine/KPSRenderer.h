#pragma once
#include <libVocalShaper.h>
#include <JuceHeader.h>

class KPSRenderer final : public vocalshaper::RendererBase
{
public:
	KPSRenderer() = default;
	~KPSRenderer() override = default;

	double renderSentence(
		juce::AudioBuffer<float>& buffer, const vocalshaper::Track* track,
		const std::tuple<int, int> sentence,
		const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate, uint32_t frameLength) const override;

	double renderBasePitch(
		PitchArray& pitchArray, const vocalshaper::Track* track,
		const std::tuple<int, int> sentence,
		const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
		uint32_t curveQuantification, uint32_t frameLength) const override;

	double getPitch(
		PitchArray& pitchArray, const juce::AudioBuffer<float>& audio,
		const vocalshaper::Track* track, const std::tuple<int, int> sentence,
		const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
		uint32_t curveQuantification, uint32_t frameLength) const override;

	double getDynamic(
		PitchArray& dynamicArray, const juce::AudioBuffer<float>& audio,
		const vocalshaper::Track* track, const std::tuple<int, int> sentence,
		const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
		uint32_t curveQuantification, uint32_t frameLength) const override;

	double getSpectrogrum(
		SpectrogrumArray& spectArray, const juce::AudioBuffer<float>& audio,
		const vocalshaper::Track* track, const std::tuple<int, int> sentence,
		const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
		uint32_t frameLength, uint32_t scaleRatio) const override;

	double getFormantrogrum(
		SpectrogrumArray& formantArray, const juce::AudioBuffer<float>& audio,
		const vocalshaper::Track* track, const std::tuple<int, int> sentence,
		const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
		uint32_t frameLength, uint32_t scaleRatio) const override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KPSRenderer)
};