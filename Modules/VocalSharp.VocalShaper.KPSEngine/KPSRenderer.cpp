#include "KPSRenderer.h"

double KPSRenderer::renderSentence(
	juce::AudioBuffer<float>& buffer, const vocalshaper::Track* track,
	const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate, uint32_t frameLength) const
{
	//TODO
	return 0;
}

double KPSRenderer::renderBasePitch(
	PitchArray& pitchArray, const vocalshaper::Track* track,
	const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
	uint32_t curveQuantification, uint32_t frameLength) const
{
	//TODO
	return 0;
}

double KPSRenderer::getPitch(
	PitchArray& pitchArray, const juce::AudioBuffer<float>& audio,
	const vocalshaper::Track* track, const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
	uint32_t curveQuantification, uint32_t frameLength) const
{
	//TODO
	return 0;
}

double KPSRenderer::getDynamic(
	PitchArray& dynamicArray, const juce::AudioBuffer<float>& audio,
	const vocalshaper::Track* track, const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
	uint32_t curveQuantification, uint32_t frameLength) const
{
	//TODO
	return 0;
}

double KPSRenderer::getSpectrogrum(
	SpectrogrumArray& spectArray, const juce::AudioBuffer<float>& audio,
	const vocalshaper::Track* track, const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
	uint32_t frameLength, uint32_t scaleRatio) const
{
	//TODO
	return 0;
}

double KPSRenderer::getFormantrogrum(
	SpectrogrumArray& formantArray, const juce::AudioBuffer<float>& audio,
	const vocalshaper::Track* track, const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
	uint32_t frameLength, uint32_t scaleRatio) const
{
	//TODO
	return 0;
}