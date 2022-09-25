#include "ClipBoard.h"

void ClipBoard::copyAndCut(ClipBoard::ArrayType&& array)
{
	juce::ScopedWriteLock locker(this->lock);
	ArrayType temp;
	temp.swapWith(array);
	this->list.add(std::move(temp));
	if (this->list.size() > 1) {
		this->list.swap(1, this->list.size() - 1);
	}
}

const ClipBoard::ArrayType& ClipBoard::paste()
{
	juce::ScopedReadLock locker(this->lock);
	if (this->list.size() > 0) {
		return this->list.getReference(this->list.size() - 1);
	}
	return this->empty;
}

const ClipBoard::ArrayType& ClipBoard::paste(int index)
{
	juce::ScopedReadLock locker(this->lock);
	if (index >= 0 && index <this->list.size()) {
		return this->list.getReference(index);
	}
	return this->empty;
}

juce::StringArray ClipBoard::getList()
{
	juce::ScopedReadLock locker(this->lock);
	juce::StringArray result;
	for (auto& i : this->list) {
		//判断是否多元素
		bool arrayMode = false;
		if (i.size() > 1) {
			arrayMode = true;
		}

		//多元素添加列表头
		juce::String temp;
		if (arrayMode) {
			temp += juce::String::formatted("<%d> [", i.size());
		}
		for (auto o : i) {
			//Json序列化
			juce::String tempStr;
			if (vocalshaper::files::vsp3::ProtoConverter::serilazeToJson(o, tempStr, false)) {
				temp += tempStr;
				//多元素加分隔
				if (arrayMode) {
					temp += ", ";
				}
			}
		}
		if (arrayMode) {
			temp.dropLastCharacters(2);
			temp += "]";
		}
		result.add(temp);
	}
	return result;
}

void ClipBoard::clean()
{
	juce::ScopedWriteLock locker(this->lock);
	this->list.clear();
}