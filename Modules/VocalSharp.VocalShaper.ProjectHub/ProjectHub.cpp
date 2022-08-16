#include "ProjectHub.h"

bool ProjectHub::newProj(const juce::String& name, const juce::String& path)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	auto proj = this->create(name, path);
	this->projList.insert(0, proj);
	this->currentIndex = 0;
	return true;
}

bool ProjectHub::copyProj(const juce::String& name, const juce::String& path,
	const juce::String& nameSrc, const juce::String& pathSrc)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	auto proj = this->create(name, path);
	this->projList.insert(0, proj);
	this->currentIndex = 0;
	return true;
}

bool ProjectHub::openProj(const juce::String& name, const juce::String& path)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->lock);
	auto proj = this->create(name, path);
	this->projList.insert(0, proj);
	this->currentIndex = 0;
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
			if (this->currentIndex >= this->projList.size()) {
				this->currentIndex--;
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

vocalshaper::Project* ProjectHub::get(int index)
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

vocalshaper::Project* ProjectHub::create(const juce::String& name, const juce::String& path) const
{
	return new vocalshaper::Project(name, path);
}
