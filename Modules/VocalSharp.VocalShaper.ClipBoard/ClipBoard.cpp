#include "ClipBoard.h"

void ClipBoard::copyAndCut(ClipBoard::ArrayType&& array)
{
	juce::ScopedWriteLock locker(this->lock);
	auto ptrData = new juce::OwnedArray(std::move(array));
	this->list.insert(0, ptrData);
}

ClipBoard::ArrayType ClipBoard::paste()
{
	juce::ScopedReadLock locker(this->lock);
	if (this->list.size() > 0) {
		auto ptrData = this->list.getUnchecked(this->list.size() - 1);
		ArrayType temp;
		for (auto i : *ptrData) {
			temp.add(vocalshaper::ProjectCopier::copy(i));
		}
		return temp;
	}
	return ArrayType();
}

ClipBoard::ArrayType ClipBoard::paste(int index)
{
	juce::ScopedReadLock locker(this->lock);
	if (index >= 0 && index <this->list.size()) {
		auto ptrData = this->list.getUnchecked(index);
		ArrayType temp;
		for (auto i : *ptrData) {
			temp.add(vocalshaper::ProjectCopier::copy(i));
		}
		return temp;
	}
	return ArrayType();
}

juce::StringArray ClipBoard::getList()
{
	juce::ScopedReadLock locker(this->lock);
	juce::StringArray result;
	for (auto i : this->list) {
		//判断是否多元素
		bool arrayMode = false;
		if (i->size() > 1) {
			arrayMode = true;
		}

		//多元素添加列表头
		juce::String temp;
		if (arrayMode) {
			temp += juce::String::formatted("<%d> [", i->size());
		}
		for (auto o : *i) {
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

int ClipBoard::size()
{
	juce::ScopedReadLock locker(this->lock);
	return this->list.size();
}

void ClipBoard::clean()
{
	juce::ScopedWriteLock locker(this->lock);
	this->list.clear();
}