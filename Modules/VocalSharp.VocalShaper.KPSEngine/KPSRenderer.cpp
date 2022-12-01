#include "KPSRenderer.h"
#include "libJModule.h"
#include "KPSEngine.h"

KPSRenderer::KPSRenderer(const KPSEngine* parent)
	: parent(parent)
{}


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

	//加载声库
	auto singer = vocalshaper::TrackDAO::getSinger(track);
	auto style = vocalshaper::TrackDAO::getStyle(track);
	auto singerInfo = vocalshaper::SingerHub::getInfo(singer);
	if (singerInfo.id != singer) {
		return 0;
	}
	if (!singerInfo.styles.contains(style)) {
		if (singerInfo.styles.size() == 0) {
			return 0;
		}
		style = singerInfo.styles.getReference(0);
	}
	auto handle = this->loadSinger(singer, style);

	//取轨道参数
	auto paramList = this->parent->getParamList();

	auto pitCurve = vocalshaper::EngineHelper::findCurve(
		track, vocalshaper::EngineHelper::makeFullID("PIT"));
	auto pitParam = vocalshaper::EngineHelper::findParam(
		track, vocalshaper::EngineHelper::makeFullID("PIT"));
	auto pitInfo = vocalshaper::EngineHelper::findParamInfo(paramList, "PIT");

	auto dynCurve = vocalshaper::EngineHelper::findCurve(
		track, vocalshaper::EngineHelper::makeFullID("DYN"));
	auto dynParam = vocalshaper::EngineHelper::findParam(
		track, vocalshaper::EngineHelper::makeFullID("DYN"));
	auto dynInfo = vocalshaper::EngineHelper::findParamInfo(paramList, "DYN");

	auto opeInfo = vocalshaper::EngineHelper::findParamInfo(paramList, "OPE");

	//遍历音符
	for (int i = std::get<0>(sentence); i <= std::get<1>(sentence); i++) {
		//如果渲染中止
		if (juce::Thread::currentThreadShouldExit()) {
			return 0;
		}

		auto note = vocalshaper::TrackDAO::getNote(track, i);
		if (note) {
			//计算音符始末位置
			auto noteStartBeat = vocalshaper::NoteDAO::getSt(note);
			auto noteEndBeat =
				vocalshaper::NoteDAO::getSt(note) + vocalshaper::NoteDAO::getLength(note);
			
			//取音符参数
			auto opeParam = vocalshaper::EngineHelper::findParam(
				note, vocalshaper::EngineHelper::makeFullID("OPE"));

			//获取音符属性
			auto pitch = vocalshaper::NoteDAO::getPitch(note);

			//生成音频
			this->KarPlusStrongRenderNote(
				buffer, handle, noteStartBeat, noteEndBeat, pitch,
				sampleRate, startBeat, *tempoTemp,
				pitCurve, dynCurve, pitParam, dynParam, opeParam,
				pitInfo, dynInfo, opeInfo);
		}
	}

	//偏移量，单位秒
	return 
		((int)(startTime * sampleRate) / (double)sampleRate) - startTime;
}

double KPSRenderer::renderBasePitch(
	PitchArray& pitchArray, const vocalshaper::Track* track,
	const std::tuple<int, int> sentence,
	const vocalshaper::TempoTemp* tempoTemp, uint32_t sampleRate,
	uint32_t curveQuantification, uint32_t frameLength) const
{
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
	return 0;
}

KPSRenderer::SingerHandle::SingerHandle(
	KPSRenderer::SingerTempType& temp, const juce::String& singer, const juce::String& style,
	juce::CriticalSection& lock,
	const std::function<juce::Array<uint16_t>(const juce::String&, const juce::String&)> loadFunc)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(lock);
	
	this->id << singer << juce::String(".") << style;

	auto it = temp.find(id);
	if (it == temp.end()) {
		auto result = temp.emplace(std::make_pair(this->id, std::make_tuple(0, std::move(loadFunc(singer, style)))));
		if (result.second) {
			it = result.first;
		}
	}

	if (it != temp.end()) {
		this->temp = &temp;
		this->lock = &lock;
		this->data = &(it->second);
		std::get<0>(it->second)++;
	}
}

KPSRenderer::SingerHandle::SingerHandle(KPSRenderer::SingerHandle&& other) noexcept
{
	juce::GenericScopedLock<juce::CriticalSection> locker(*(other.lock));

	this->temp = other.temp;
	this->id = other.id;
	this->lock = other.lock;
	this->data = other.data;

	other.temp = nullptr;
	other.lock = nullptr;
	other.data = nullptr;
}

KPSRenderer::SingerHandle& KPSRenderer::SingerHandle::operator=(KPSRenderer::SingerHandle&& other) noexcept
{
	if (this != &other) {
		juce::GenericScopedLock<juce::CriticalSection> locker(*(other.lock));

		this->temp = other.temp;
		this->id = other.id;
		this->lock = other.lock;
		this->data = other.data;

		other.temp = nullptr;
		other.lock = nullptr;
		other.data = nullptr;
	}
	return *this;
}

KPSRenderer::SingerHandle::~SingerHandle()
{
	if (this->data && this->temp && this->lock) {
		juce::GenericScopedLock<juce::CriticalSection> locker(*(this->lock));
		std::get<0>(*(this->data))--;

		if (std::get<0>(*(this->data)) <= 0) {
			this->temp->erase(this->id);
		}
	}
}

const KPSRenderer::SingerHandle KPSRenderer::loadSinger(
	const juce::String& singer, const juce::String& style) const
{
	auto loadFunc = [](const juce::String& singer, const juce::String& style)->juce::Array<uint16_t> {
		auto info = jmadf::FindModule(singer);
		if (!info) {
			return juce::Array<uint16_t>();
		}

		juce::String path = info->path + "/.database/" + style + ".nsdb";
		juce::File file(path);
		juce::FileInputStream stream(file);
		if (stream.failedToOpen()) {
			return juce::Array<uint16_t>();
		}

		juce::Array<uint16_t> result;
		while (stream.getNumBytesRemaining() >= 2) {
			result.add(stream.readShort());
		}

		return result;
	};

	return std::move(
		KPSRenderer::SingerHandle(
			this->dbTemp, singer, style, this->dbTempLock, loadFunc));
}

void KPSRenderer::KarPlusStrongRenderNote(
	juce::AudioBuffer<float>& buffer, const SingerHandle& singerData,
	double noteStartBeat, double noteEndBeat, uint8_t pitch, uint32_t sampleRate,
	double startBeat, const vocalshaper::TempoTemp& tempoTemp,
	const vocalshaper::Curve* pitCurve, const vocalshaper::Curve* dynCurve,
	const vocalshaper::Param* pitParam, const vocalshaper::Param* dynParam,
	const vocalshaper::Param* opeParam,
	const vocalshaper::ParamInfo& pitInfo, const vocalshaper::ParamInfo& dynInfo,
	const vocalshaper::ParamInfo& opeInfo) const
{
	//计算始末位置
	auto startTime = tempoTemp.get_t(startBeat);
	auto noteStartTime = tempoTemp.get_t(noteStartBeat);
	auto noteEndTime = tempoTemp.get_t(noteEndBeat);
	int startPointIndex = (noteStartTime - startTime) * sampleRate;
	int endPointIndex = (noteEndTime - startTime) * sampleRate;

	//TODO 单元合成
}
