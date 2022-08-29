#include "ProjectHub.h"

bool ProjectHub::newProj(const juce::String& name, const juce::String& path)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);

	int openedIndex = this->findOpened(name, path);
	if (openedIndex >= 0) {
		this->projList.move(openedIndex, 0);
		this->currentIndex = 0;
		return true;
	}

	auto proj = this->create(name, path);
	this->projList.insert(0, proj);
	this->currentIndex = 0;
	//TODO
	return true;
}

bool ProjectHub::copyProj(const juce::String& name, const juce::String& path,
	const juce::String& nameSrc, const juce::String& pathSrc)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);

	int openedIndex = this->findOpened(name, path);
	if (openedIndex >= 0) {
		this->projList.move(openedIndex, 0);
		this->currentIndex = 0;
		return true;
	}

	auto proj = this->create(name, path);
	this->projList.insert(0, proj);
	this->currentIndex = 0;
	//TODO
	return true;
}

bool ProjectHub::openProj(const juce::String& name, const juce::String& path)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);

	int openedIndex = this->findOpened(name, path);
	if (openedIndex >= 0) {
		this->projList.move(openedIndex, 0);
		this->currentIndex = 0;
		return true;
	}

	auto proj = this->create(name, path);
	this->projList.insert(0, proj);
	this->currentIndex = 0;
	//TODO
	return true;
}

void ProjectHub::setCurrent(int index)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		this->currentIndex = index;
	}
}

void ProjectHub::setCurrentAndToFront(int index)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	if (index > 0 && index < this->projList.size()) {
		this->projList.move(index, 0);
		this->currentIndex = 0;
	}
}

void ProjectHub::close(int index)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		this->projList.remove(index);
		if (this->currentIndex == index) {
			//关闭的是当前文档
			if (this->currentIndex > 0) {
				//前面有文档则当前文档为关闭的前一个文档
				this->currentIndex--;
			}
			else if (this->projList.size() > 0) {
				//后面有文档则后一个文档为当前文档
			}
			else {
				//全部关闭后置当前文档为-1
				this->currentIndex = -1;
			}
		}
		else if (this->currentIndex > index) {
			//关闭的文档在当前文档以前
			this->currentIndex--;
		}
	}
}

bool ProjectHub::checkForClose(int index)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		return this->projList.getUnchecked(index)->getSaved();
	}
	return true;
}

vocalshaper::ProjectProxy* ProjectHub::get(int index)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		return this->projList.getUnchecked(index);
	}
	return nullptr;
}

int ProjectHub::getCurrent()
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	return this->currentIndex;
}

int ProjectHub::getSize()
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	return this->projList.size();
}

void ProjectHub::save(int index)
{
	//TODO
}

vocalshaper::ProjectProxy* ProjectHub::create(const juce::String& name, const juce::String& path) const
{
	return new vocalshaper::ProjectProxy(name, path);
}

int ProjectHub::findOpened(const juce::String& name, const juce::String& path) const
{
	for (int i = 0; i < this->projList.size(); i++) {
		auto item = this->projList.getUnchecked(i);
		if (item->getName() == name && item->getPath() == path) {
			return i;
		}
	}
	return -1;
}
