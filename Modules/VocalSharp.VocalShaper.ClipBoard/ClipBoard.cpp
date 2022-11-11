#include "ClipBoard.h"

ClipBoard::ArrayType* ClipBoard::add()
{
	juce::ScopedWriteLock locker(this->lock);
	return this->list.insert(0, new ArrayType);
}

const ClipBoard::ArrayType* ClipBoard::get()
{
	return this->get(0);
}

const ClipBoard::ArrayType* ClipBoard::get(int index)
{
	juce::ScopedReadLock locker(this->lock);
	if (index >= 0 && index <this->list.size()) {
		return this->list.getUnchecked(index);
	}
	return nullptr;
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

void ClipBoard::acceptCopyAndDelete(
	const juce::String& caller,
	const vocalshaper::EditorBase* editor)
{
	juce::ScopedWriteLock locker(this->funcLock);
	if (this->cadMethod.editor) {
		if (this->cadMethod.editor != editor) {
			this->cadMethod.editor->onUnselectAll();
		}
	}
	this->cadMethod = { caller, const_cast<vocalshaper::EditorBase*>(editor) };
}

void ClipBoard::acceptPaste(
	const juce::String& caller,
	const vocalshaper::EditorBase* editor)
{
	juce::ScopedWriteLock locker(this->funcLock);
	if (this->pMethod.editor) {
		if (this->pMethod.editor != editor) {
			this->pMethod.editor->onUnselectAll();
		}
	}
	this->pMethod = { caller, const_cast<vocalshaper::EditorBase*>(editor) };
}

void ClipBoard::unacceptCopyAndDelete(
	const vocalshaper::EditorBase* editor)
{
	juce::ScopedWriteLock locker(this->funcLock);
	if (this->cadMethod.editor == editor) {
		this->cadMethod.editor->onUnselectAll();
		this->cadMethod = 
		{ juce::String(), nullptr };
	}
}

void ClipBoard::unacceptPaste(
	const vocalshaper::EditorBase* editor)
{
	juce::ScopedWriteLock locker(this->funcLock);
	if (this->pMethod.editor == editor) {
		this->pMethod.editor->onUnselectAll();
		this->pMethod = { juce::String(), nullptr };
	}
}

void ClipBoard::release(const juce::String& caller)
{
	juce::ScopedWriteLock locker(this->funcLock);
	if (this->cadMethod.caller == caller) {
		this->cadMethod = 
		{ juce::String(), nullptr };
	}
	if (this->pMethod.caller == caller) {
		this->pMethod = { juce::String(), nullptr };
	}
}

bool ClipBoard::couldCopyAndDelete()
{
	juce::ScopedReadLock locker(this->funcLock);
	return this->cadMethod.editor;
}

bool ClipBoard::couldPaste()
{
	juce::ScopedReadLock locker(this->funcLock);
	return this->pMethod.editor && (this->size() > 0);
}

void ClipBoard::sendCopy()
{
	juce::ScopedReadLock locker(this->funcLock);
	if (this->couldCopyAndDelete()) {
		if (this->cadMethod.editor) {
			auto ptr = this->add();
			if (ptr) {
				this->cadMethod.editor->onCopy(*ptr);
			}
			else {
				jassertfalse;
			}
		}
		else {
			jassertfalse;
		}
	}
}

void ClipBoard::sendDelete()
{
	juce::ScopedReadLock locker(this->funcLock);
	if (this->couldCopyAndDelete()) {
		if (this->cadMethod.editor) {
			this->cadMethod.editor->onDelete();
			this->cadMethod.editor->onUnselectAll();
		}
		else {
			jassertfalse;
		}
	}
}

void ClipBoard::sendSelectAll()
{
	juce::ScopedReadLock locker(this->funcLock);
	if (this->couldCopyAndDelete()) {
		if (this->cadMethod.editor) {
			this->cadMethod.editor->onSelectAll();
		}
		else {
			jassertfalse;
		}
	}
}

void ClipBoard::sendPaste()
{
	this->sendPasteWithIndex(0);
}

void ClipBoard::sendPasteWithIndex(int index)
{
	juce::ScopedReadLock locker(this->funcLock);
	if (this->couldPaste()) {
		if (index >= 0 && index < this->size()) {
			if (this->pMethod.editor) {
				auto ptr = this->get(index);
				if (ptr) {
					this->pMethod.editor->onPaste(*ptr);
					this->pMethod.editor->onUnselectAll();
				}
				else {
					jassertfalse;
				}
			}
			else {
				jassertfalse;
			}
		}
	}
}

bool ClipBoard::couldCopyToSystem()
{
	juce::ScopedReadLock locker(this->funcLock);
	return this->couldCopyAndDelete();
}

bool ClipBoard::couldPasteFromSystem()
{
	juce::ScopedReadLock locker(this->funcLock);
	if (!this->couldPaste()) {
		return false;
	}

	juce::String str = juce::SystemClipboard::getTextFromClipboard();
	juce::var var;
	return juce::JSON::parse(str, var).wasOk();
}

void ClipBoard::sendCopyToSystem()
{
	juce::ScopedReadLock locker(this->funcLock);
	if (this->couldCopyAndDelete()) {
		if (this->cadMethod.editor) {
			auto temp = std::make_unique<ArrayType>();
			this->cadMethod.editor->onCopy(*temp);
			if (temp->size() > 0) {
				//判断是否多元素
				bool arrayMode = false;
				if (temp->size() > 1) {
					arrayMode = true;
				}

				//多元素添加列表头
				juce::String tempS;
				if (arrayMode) {
					tempS += "[\n";
				}
				for (auto o : *temp) {
					//Json序列化
					juce::String tempStr;
					if (vocalshaper::files::vsp3::ProtoConverter::serilazeToJson(
						o, tempStr, true)) {
						tempS += tempStr;
						//多元素加分隔
						if (arrayMode) {
							tempS += "\n,\n";
						}
					}
				}
				//多元素添尾
				if (arrayMode) {
					tempS.dropLastCharacters(2);
					tempS += "]";
				}
				juce::SystemClipboard::copyTextToClipboard(tempS);
			}
		}
		else {
			jassertfalse;
		}
	}
}

void ClipBoard::sendPasteFromSystem()
{
	juce::ScopedReadLock locker(this->funcLock);
	if (this->couldPaste()) {
		if (this->pMethod.editor) {
			juce::String str = juce::SystemClipboard::getTextFromClipboard();
			juce::StringArray clipList;
			{
				//获取剪贴板数据
				juce::var var;
				if (!juce::JSON::parse(str, var).wasOk()) {
					return;
				}
				if (var.isArray()) {
					auto array = var.getArray();
					for (auto& i : *array) {
						clipList.add(juce::JSON::toString(i, true));
					}
				}
				else {
					clipList.add(str);
				}
			}

			//对数据类型进行判断转换
			auto temp = std::make_unique<ArrayType>();
			for (auto& s : clipList) {
				auto object = std::make_unique<vocalshaper::SerializableProjectStructure>(
					vocalshaper::SerializableProjectStructure::Type::Empty);
				if (vocalshaper::files::vsp3::ProtoConverter::parseFromJson(
					s, object.get())) {
					temp->add(object.release());
				}
			}

			//粘贴数据
			if (temp->size() > 0) {
				this->pMethod.editor->onPaste(*temp);
				this->pMethod.editor->onUnselectAll();
			}
		}
		else {
			jassertfalse;
		}
	}
}
