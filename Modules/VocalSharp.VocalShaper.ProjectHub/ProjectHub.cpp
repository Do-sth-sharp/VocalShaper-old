#include "ProjectHub.h"
#include <libJModule.h>
#include <libVocalShaper.h>

bool ProjectHub::newProj(const juce::String& name, const juce::String& path)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);

	int openedIndex = this->findOpened(name, path);
	if (openedIndex >= 0) {
		this->projList.move(openedIndex, 0);
		this->currentIndex = 0;
		this->callNotice();
		return true;
	}

	auto proj = this->create(name, path);
	auto metaBackup = proj->getMeta()->backup();
	proj->getMeta()->wannaSave();
	bool result = false;
	jmadf::CallInterface<vocalshaper::ProjectProxy*, bool&>(
		"VocalSharp.VocalShaper.ProjectIO", "WriteProject",
		proj, result
		);
	if (!result) {
		//proj->getMeta()->recover(metaBackup);
		//proj->noticeClose();
		delete proj;
		return false;
	}
	vocalshaper::ProjectDAO::save(proj->getPtr());
	proj->noticeSave();

	this->projList.insert(0, proj);
	this->currentIndex = 0;
	this->callNotice();

	proj->getProcesser()->processEvent(
		std::make_unique<vocalshaper::actions::ProjectAction>(
			vocalshaper::actions::ProjectAction::TargetType(), proj));

	return true;
}

bool ProjectHub::copyProj(const juce::String& name, const juce::String& path,
	const juce::String& nameSrc, const juce::String& pathSrc)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);

	int openedIndex = this->findOpened(name, path);
	if (openedIndex >= 0) {
		this->projList.move(openedIndex, 0);
		this->currentIndex = 0;
		this->callNotice();
		return true;
	}

	auto proj = this->create(nameSrc, pathSrc);
	bool result = false;
	jmadf::CallInterface<vocalshaper::ProjectProxy*, bool&>(
		"VocalSharp.VocalShaper.ProjectIO", "ReadProject",
		proj, result
		);
	if (!result) {
		delete proj;
		return false;
	}

	proj->changeUrl(name, path);

	auto metaBackup = proj->getMeta()->backup();
	proj->getMeta()->wannaSave();
	result = false;
	jmadf::CallInterface<vocalshaper::ProjectProxy*, bool&>(
		"VocalSharp.VocalShaper.ProjectIO", "WriteProject",
		proj, result
		);
	if (!result) {
		//proj->getMeta()->recover(metaBackup);
		//proj->noticeClose();
		delete proj;
		return false;
	}
	vocalshaper::ProjectDAO::save(proj->getPtr());
	proj->noticeSave();

	this->projList.insert(0, proj);
	this->currentIndex = 0;
	this->callNotice();

	proj->getProcesser()->processEvent(
		std::make_unique<vocalshaper::actions::ProjectAction>(
			vocalshaper::actions::ProjectAction::TargetType(), proj));

	return true;
}

bool ProjectHub::openProj(const juce::String& name, const juce::String& path)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);

	int openedIndex = this->findOpened(name, path);
	if (openedIndex >= 0) {
		this->projList.move(openedIndex, 0);
		this->currentIndex = 0;
		this->callNotice();
		return true;
	}

	auto proj = this->create(name, path);

	bool result = false;
	jmadf::CallInterface<vocalshaper::ProjectProxy*, bool&>(
		"VocalSharp.VocalShaper.ProjectIO", "ReadProject",
		proj, result
		);
	if (!result) {
		delete proj;
		return false;
	}
	vocalshaper::ProjectDAO::save(proj->getPtr());

	this->projList.insert(0, proj);
	this->currentIndex = 0;
	this->callNotice();

	proj->getProcesser()->processEvent(
		std::make_unique<vocalshaper::actions::ProjectAction>(
			vocalshaper::actions::ProjectAction::TargetType(), proj));

	return true;
}

void ProjectHub::setCurrent(int index)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		this->currentIndex = index;
		this->callNotice();
	}
}

void ProjectHub::setCurrentAndToFront(int index)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	if (index > 0 && index < this->projList.size()) {
		this->projList.move(index, 0);
		this->currentIndex = 0;
		this->callNotice();
	}
}

void ProjectHub::close(int index)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		auto proj = this->projList.removeAndReturn(index);
		proj->noticeClose();
		delete proj;
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
		this->callNotice();
	}
}

bool ProjectHub::checkForClose(int index)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		return this->projList.getUnchecked(index)->getSaved();
	}
	return true;
}

vocalshaper::ProjectProxy* ProjectHub::get(int index)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		return this->projList.getUnchecked(index);
	}
	return nullptr;
}

int ProjectHub::getCurrent()
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	return this->currentIndex;
}

int ProjectHub::getSize()
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	return this->projList.size();
}

bool ProjectHub::save(int index)
{
	juce::GenericScopedLock<juce::CriticalSection> locker(this->lock);
	if (index >= 0 && index < this->projList.size()) {
		auto ptr = this->projList.getUnchecked(index);
		auto metaBackup = ptr->getMeta()->backup();
		ptr->getMeta()->wannaSave();
		bool result = false;
		jmadf::CallInterface<vocalshaper::ProjectProxy*, bool&>(
			"VocalSharp.VocalShaper.ProjectIO", "WriteProject",
			ptr, result
			);
		if (result) {
			vocalshaper::ProjectDAO::save(ptr->getPtr());
			ptr->noticeSave();
			return true;
		}
		ptr->getMeta()->recover(metaBackup);
	}
	return false;
}

void ProjectHub::addNotice(const juce::String& caller, const ChangeNoticeFunction& func)
{
	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.insert(caller);

	juce::ScopedWriteLock locker(this->funcLock);
	this->funcList.insert(std::make_pair(caller, func));
}

void ProjectHub::release(const juce::String& caller)
{
	juce::ScopedWriteLock sLock(this->setLock);
	if (this->mSet.find(caller) == this->mSet.end()) {
		return;
	}
	this->mSet.erase(caller);

	juce::ScopedWriteLock locker(this->funcLock);
	for (auto it = this->funcList.begin(); it != this->funcList.end();) {
		if ((*it).first == caller) {
			it = this->funcList.erase(it);
			continue;
		}
		it++;
	}
}

vocalshaper::ProjectProxy* ProjectHub::create(const juce::String& name, const juce::String& path) const
{
	auto ptr = new vocalshaper::ProjectProxy(name, path);

	jmadf::CallInterface<vocalshaper::ProjectProxy*>(
		"VocalSharp.VocalShaper.CallbackReactor", "Process", ptr);

	return ptr;
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

void ProjectHub::callNotice()
{
	juce::ScopedReadLock locker(this->funcLock);
	const vocalshaper::ProjectProxy* ptr = this->get(this->currentIndex);

	for (auto& i : this->funcList) {
		i.second(ptr);
	}
}
