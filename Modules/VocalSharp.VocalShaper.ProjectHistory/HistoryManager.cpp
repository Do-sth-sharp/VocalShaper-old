#include "HistoryManager.h"
#include <libJModule.h>

HistoryManager::HistoryManager()
{
	juce::ScopedWriteLock locker(this->lock);

	juce::var* config = nullptr;
	bool ok = false;
	jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
		"WuChang.JMADF.Configs", "GetReference",
		"config", config, ok
		);
	if (ok && config->isObject()) {
		this->maxCount = (*config)["MaxCount"];
	}

	juce::File hisFile(jmadf::GetModuleInfo()->path + this->path);
	if (hisFile.existsAsFile()) {
		juce::var dataTemp = juce::JSON::parse(hisFile);
		if (dataTemp.isArray()) {
			this->data = *dataTemp.getArray();
			this->cut();
			this->filter();
		}
	}
}

HistoryManager::~HistoryManager()
{

}

int HistoryManager::getSize()
{
	juce::ScopedReadLock locker(this->lock);
	
	int count = 0;
	for (auto i : this->hide) {
		count += (!i);
	}

	return count;
}

const juce::String HistoryManager::getName(int index)
{
	juce::ScopedReadLock locker(this->lock);

	for (int i = 0, j = 0; i < this->data.size(); i++) {
		//i:数组计数器
		//j:显示数组计数器
		if (j == index && !this->hide[i]) {
			juce::var& obj = this->data.getReference(i);
			if (obj.isObject()) {
				juce::var dataRef = obj["name"];
				if (dataRef.isString()) {
					return dataRef.toString();
				}
			}
			break;
		}
		if (!this->hide[i]) {
			j++;
		}
	}
	return juce::String();
}

const juce::String HistoryManager::getPath(int index)
{
	juce::ScopedReadLock locker(this->lock);

	for (int i = 0, j = 0; i < this->data.size(); i++) {
		//i:数组计数器
		//j:显示数组计数器
		if (j == index && !this->hide[i]) {
			juce::var& obj = this->data.getReference(i);
			if (obj.isObject()) {
				juce::var dataRef = obj["path"];
				if (dataRef.isString()) {
					return dataRef.toString();
				}
			}
			break;
		}
		if (!this->hide[i]) {
			j++;
		}
	}
	return juce::String();
}

const juce::String HistoryManager::getTime(int index)
{
	juce::ScopedReadLock locker(this->lock);

	for (int i = 0, j = 0; i < this->data.size(); i++) {
		//i:数组计数器
		//j:显示数组计数器
		if (j == index && !this->hide[i]) {
			juce::var& obj = this->data.getReference(i);
			if (obj.isObject()) {
				juce::var dataRef = obj["time"];
				if (dataRef.isString()) {
					return dataRef.toString();
				}
			}
			break;
		}
		if (!this->hide[i]) {
			j++;
		}
	}
	return juce::String();
}

void HistoryManager::add(const juce::String& name, const juce::String& path)
{
	juce::ScopedWriteLock locker(this->lock);

	juce::var objTemp = new juce::DynamicObject;
	auto ptrTemp = reinterpret_cast<juce::DynamicObject*>(objTemp.getObject());
	ptrTemp->setProperty("name", name);
	ptrTemp->setProperty("path", path);
	ptrTemp->setProperty("time", this->createTime());
	for (int i = 0; i < this->data.size();) {
		juce::var& obj = this->data.getReference(i);
		if (obj.isObject()) {
			juce::var nameRef = obj["name"];
			juce::var pathRef = obj["path"];
			if (nameRef.isString() && pathRef.isString()) {
				if (nameRef.toString() == name &&
					pathRef.toString() == path) {
					this->data.remove(i);
					this->hide.remove(i);
					continue;
				}
			}
		}
		i++;
	}
	this->data.insert(0, objTemp);
	this->hide.insert(0, !this->match(objTemp));
	this->cut();
	this->save();
}

void HistoryManager::open(int index)
{
	juce::ScopedWriteLock locker(this->lock);
	
	for (int i = 0, j = 0; i < this->data.size(); i++) {
		//i:数组计数器
		//j:显示数组计数器
		if (j == index && !this->hide[i]) {
			juce::var objTemp = this->data[i];
			bool hideTemp = this->hide[i];
			this->data.remove(i);
			this->hide.remove(i);
			if (objTemp.isObject()) {
				reinterpret_cast<juce::DynamicObject*>(objTemp.getObject())
					->setProperty("time", this->createTime());
			}
			this->data.insert(0, objTemp);
			this->hide.insert(0, hideTemp);
			this->save();
			break;
		}
		if (!this->hide[i]) {
			j++;
		}
	}
}

void HistoryManager::remove(int index)
{
	juce::ScopedWriteLock locker(this->lock);

	for (int i = 0, j = 0; i < this->data.size(); i++) {
		//i:数组计数器
		//j:显示数组计数器
		if (j == index && !this->hide[i]) {
			this->data.remove(i);
			this->hide.remove(i);
			this->save();
			break;
		}
		if (!this->hide[i]) {
			j++;
		}
	}
}

void HistoryManager::setFilter(const juce::String& filter)
{
	juce::ScopedWriteLock locker(this->lock);

	if (this->filterStr != filter) {
		this->filterStr = filter;
		this->filter();
	}
}

void HistoryManager::save()
{
	juce::File hisFile(jmadf::GetModuleInfo()->path + this->path);
	juce::var dataTemp(this->data);
	hisFile.replaceWithText(
		juce::JSON::toString(dataTemp)
	);
}

void HistoryManager::cut()
{
	if (this->hide.size() > this->maxCount) {
		this->hide.resize(this->maxCount);
	}
	if (this->data.size() > this->maxCount) {
		this->data.resize(this->maxCount);
		this->save();
	}
}

bool HistoryManager::match(juce::var& object)
{
	juce::var objTemp = object["name"];
	if (objTemp.isString() && objTemp.toString().contains(this->filterStr)) {
		return true;
	}
	return false;
}

void HistoryManager::filter()
{
	this->hide.resize(this->data.size());
	for (int i = 0; i < this->data.size(); i++) {
		this->hide.getReference(i) = !this->match(this->data.getReference(i));
	}
}

const juce::String HistoryManager::createTime()
{
	return juce::Time::getCurrentTime().formatted("%Y-%m-%d %H:%M:%S");
}
