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
	using SingerTempType = std::map<juce::String, std::tuple<int, juce::Array<uint16_t>>>;
	mutable SingerTempType dbTemp;
	mutable juce::CriticalSection dbTempLock;

	struct SingerHandle final
	{
		SingerHandle() = delete;
		explicit SingerHandle(
			SingerTempType& temp, const juce::String& singer, const juce::String& style,
			juce::CriticalSection& lock,
			const std::function<juce::Array<uint16_t>(const juce::String&, const juce::String&)> loadFunc);
		SingerHandle(SingerHandle&& other);
		SingerHandle& operator=(SingerHandle&& other);
		~SingerHandle();

	private:
		SingerTempType* temp;
		juce::String id;
		juce::CriticalSection* lock;
		std::tuple<int, juce::Array<uint16_t>>* data;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SingerHandle)
	};

	const SingerHandle loadSinger(const juce::String& singer, const juce::String& style) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KPSRenderer)
};