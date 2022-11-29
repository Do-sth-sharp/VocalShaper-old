#include "KPSRenderer.h"

double KPSRenderer::renderSentence(
	juce::AudioBuffer<float>& buffer, const vocalshaper::Track* track,
	const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate, uint32_t frameLength) const
{
	if (!track || !tempoTemp) {
		return 0;
	}

	//获得渲染区始末位置
	auto noteFirst = vocalshaper::TrackDAO::getNote(track, std::get<0>(sentence));
	auto noteEnd = vocalshaper::TrackDAO::getNote(track, std::get<1>(sentence));
	if (!noteFirst || !noteEnd) {
		return 0;
	}
	auto startBeat = vocalshaper::NoteDAO::getSt(noteFirst);
	auto endBeat =
		vocalshaper::NoteDAO::getSt(noteEnd) + vocalshaper::NoteDAO::getLength(noteEnd);
	auto startTime = tempoTemp->get_t(startBeat);
	auto endTime = tempoTemp->get_t(endBeat);

	//计算音频长度并预填充
	int audioLength = std::ceil((endTime - startTime) * sampleRate);
	buffer = std::move(juce::AudioBuffer<float>(1, audioLength));

	//遍历音符
	for (int i = std::get<0>(sentence); i <= std::get<1>(sentence); i++) {
		auto note = vocalshaper::TrackDAO::getNote(track, i);
		if (note) {
			//计算音符始末位置
			auto noteStartBeat = vocalshaper::NoteDAO::getSt(note);
			auto noteEndBeat =
				vocalshaper::NoteDAO::getSt(note) + vocalshaper::NoteDAO::getLength(note);
			auto noteStartTime = tempoTemp->get_t(noteStartBeat);
			auto noteEndTime = tempoTemp->get_t(noteEndBeat);

			//计算音符控制区
			int startPointIndex = (noteStartTime - startTime) * sampleRate;
			int endPointIndex = (noteEndTime - startTime) * sampleRate;

			//TODO 生成音频
		}
	}

	return 0;
}

double KPSRenderer::renderBasePitch(
	PitchArray& pitchArray, const vocalshaper::Track* track,
	const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
	uint32_t curveQuantification, uint32_t frameLength) const
{
	return 0;

	//if (!track) {
	//	return 0;
	//}

	////获得渲染区始末位置
	//auto noteFirst = vocalshaper::TrackDAO::getNote(track, std::get<0>(sentence));
	//auto noteEnd = vocalshaper::TrackDAO::getNote(track, std::get<1>(sentence));
	//if (!noteFirst || !noteEnd) {
	//	return 0;
	//}
	//auto startTime = vocalshaper::NoteDAO::getSt(noteFirst);
	//auto endTime =
	//	vocalshaper::NoteDAO::getSt(noteEnd) + vocalshaper::NoteDAO::getLength(noteEnd);

	////计算曲线长度并预填充
	//int pitchLength = std::ceil((endTime - startTime) * curveQuantification);
	//pitchArray.resize(pitchLength);
	//pitchArray.fill(0.f);

	////填充曲线
	//auto fillPitchArrayFunc = [&pitchArray](int start, int end, float value) {
	//	for (int i = start; i <= end; i++) {
	//		pitchArray.getReference(i) = value;
	//	}
	//};

	////遍历音符
	//for (int i = std::get<0>(sentence); i <= std::get<1>(sentence); i++) {
	//	auto note = vocalshaper::TrackDAO::getNote(track, i);
	//	if (note) {
	//		//计算音符始末位置
	//		auto noteStartTime = vocalshaper::NoteDAO::getSt(note);
	//		auto noteEndTime =
	//			vocalshaper::NoteDAO::getSt(note) + vocalshaper::NoteDAO::getLength(note);

	//		//计算音符控制区
	//		int startPointIndex = (noteStartTime - startTime) * curveQuantification;
	//		int endPointIndex = (noteEndTime - startTime) * curveQuantification;

	//		//填充曲线为音符音高
	//		auto pitch = vocalshaper::NoteDAO::getPitch(note);
	//		fillPitchArrayFunc(startPointIndex, endPointIndex, pitch);
	//	}
	//}

	////偏移量，单位四分音符时长
	//return 
	//	((int)(startTime * curveQuantification) / (double)curveQuantification) - startTime;
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
	return 0;
}