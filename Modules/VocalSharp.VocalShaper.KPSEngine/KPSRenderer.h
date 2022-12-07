#pragma once
#include <libVocalShaper.h>
#include <JuceHeader.h>

class KPSEngine;

class KPSRenderer final : public vocalshaper::RendererBase
{
public:
	explicit KPSRenderer(const KPSEngine* parent);
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
	using SingerTempType = std::map<juce::String, std::tuple<int, juce::Array<float>>>;
	mutable SingerTempType dbTemp;
	mutable juce::CriticalSection dbTempLock;

	struct SingerHandle final
	{
		SingerHandle() = delete;
		explicit SingerHandle(
			SingerTempType& temp, const juce::String& singer, const juce::String& style,
			juce::CriticalSection& lock,
			const std::function<juce::Array<float>(const juce::String&, const juce::String&)> loadFunc);
		SingerHandle(SingerHandle&& other) noexcept;
		SingerHandle& operator=(SingerHandle&& other) noexcept;
		~SingerHandle();

		const juce::Array<float>* operator->() const;

	private:
		SingerTempType* temp = nullptr;
		juce::String id;
		juce::CriticalSection* lock = nullptr;
		std::tuple<int, juce::Array<float>>* data = nullptr;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SingerHandle)
	};

	const SingerHandle loadSinger(const juce::String& singer, const juce::String& style) const;

	const KPSEngine* parent = nullptr;

	void KarPlusStrongRenderNote(
		juce::AudioBuffer<float>& buffer, const SingerHandle& singerData,
		double noteStartBeat, double noteEndBeat, uint8_t pitch, uint32_t sampleRate,
		double startBeat, const vocalshaper::TempoTemp& tempoTemp,
		const vocalshaper::Curve* pitCurve, const vocalshaper::Curve* dynCurve,
		const vocalshaper::Param* pitParam, const vocalshaper::Param* dynParam,
		const vocalshaper::Param* opeParam,
		const vocalshaper::ParamInfo& pitInfo, const vocalshaper::ParamInfo& dynInfo,
		const vocalshaper::ParamInfo& opeInfo) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KPSRenderer)
};