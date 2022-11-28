#include "KPSSpliter.h"

const vocalshaper::SpliterBase::SentenceList KPSSpliter::getSentence(const vocalshaper::Track* track) const
{
	//保存结果的序列
	vocalshaper::SpliterBase::SentenceList result;

	//音符总数
	int noteSize = vocalshaper::TrackDAO::noteSize(track);

	////准备重叠符号序列
	//juce::Array<uint8_t> flagArray;
	//flagArray.resize(std::ceil(noteSize / 8.));
	//flagArray.fill(0xFFUi8);

	////设符号
	//auto setFlagFunc = [&flagArray](int index, bool flag) {
	//	auto i = std::floor(index / 8.);
	//	auto j = 7 - index % 8;

	//	auto& data = flagArray.getReference(i);
	//	if (flag) {
	//		data |= (0x01Ui8 << j);
	//	}
	//	else {
	//		data &= ~(0x01Ui8 << j);
	//	}
	//};
	////查符号
	//auto getFlagFunc = [&flagArray](int index)->bool {
	//	auto i = std::floor(index / 8.);
	//	auto j = 7 - index % 8;

	//	auto& data = flagArray.getReference(i);
	//	return data & (0x01Ui8 << j);
	//};

	////对重叠符号进行判断
	//double lastTail = -1.;
	//int lastTailIndex = -1;
	//for (int i = 0; i < noteSize; i++) {
	//	auto note = vocalshaper::TrackDAO::getNote(track, i);
	//	if (!note) {
	//		setFlagFunc(i, false);
	//		continue;
	//	}

	//	//获取音符时间
	//	auto st = vocalshaper::NoteDAO::getSt(note);
	//	auto length = vocalshaper::NoteDAO::getLength(note);

	//	//如果存在重叠
	//	if (lastTail > st) {
	//		setFlagFunc(i, false);
	//		if (lastTailIndex >= 0 && lastTailIndex < noteSize) {
	//			setFlagFunc(lastTailIndex, false);
	//		}
	//	}

	//	//如果音符尾增加
	//	if (st + length > lastTail) {
	//		lastTail = st + length;
	//		lastTailIndex = i;
	//	}
	//}

	////遍历符号表生成结果区间
	//for (int i = 0; i < noteSize; i++) {
	//	if (getFlagFunc(i)) {
	//		result.add(std::make_tuple(i, i));
	//	}
	//}

	//生成结果
	for (int i = 0; i < noteSize; i++) {
		result.add(std::make_tuple(i, i));
	}

	return result;
}
