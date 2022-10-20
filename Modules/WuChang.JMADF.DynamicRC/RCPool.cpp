#include "RCPool.h"

std::pair<size_t, void*> RCPool::get(const juce::String& moduleName, const juce::String& path)
{
	juce::GenericScopedLock<juce::SpinLock> locker(this->rcLock);
	auto& ref = this->list[path];
	if (!ref) {
		ref = std::make_shared<RCBlock>();
	}
	ref->sm.insert(moduleName);
	if (!ref->ptr) {
		juce::File file(path);
		juce::FileInputStream stream(file);
		if (stream.failedToOpen()) {
			this->list.erase(path);
			return std::make_pair(0, nullptr);
		}
		
		ref->size = stream.getTotalLength();
		ref->ptr = std::shared_ptr<char>(new char[ref->size], [](char* p) {delete[] p; });
		if (!ref->ptr) {
			this->list.erase(path);
			return std::make_pair(0, nullptr);
		}
		
		stream.read(ref->ptr.get(), ref->size);
	}
	
	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.insert(moduleName);

	return std::make_pair(ref->size, ref->ptr.get());
}

std::pair<size_t, void*> RCPool::copy(const juce::String& moduleName, const juce::String& path)
{
	auto&& p = this->get(moduleName, path);
	if (!p.second) {
		return std::make_pair(0, nullptr);
	}
	
	std::pair<size_t, void*> pair = std::make_pair(p.first, new char[p.first]);
	if (!pair.second) {
		return std::make_pair(0, nullptr);
	}
	
	std::memcpy(pair.second, p.second, p.first);
	return pair;
}

void RCPool::release(const juce::String& moduleName, const juce::String& path)
{
	juce::ScopedWriteLock sLock(this->setLock);
	this->mSet.erase(moduleName);

	juce::GenericScopedLock<juce::SpinLock> locker(this->rcLock);
	auto& ref = this->list[path];
	if (!ref) {
		ref = std::make_shared<RCBlock>();
	}
	ref->sm.erase(moduleName);
	if (ref->sm.size() == 0) {
		this->list.erase(path);
	}
}

void RCPool::destory(const juce::String& moduleName)
{
	juce::ScopedWriteLock sLock(this->setLock);
	if (this->mSet.find(moduleName) == this->mSet.end()) {
		return;
	}
	this->mSet.erase(moduleName);

	juce::GenericScopedLock<juce::SpinLock> locker(this->rcLock);
	for (auto it = this->list.begin(); it != this->list.end();) {
		auto& ref = (*it).second;
		if (!ref) {
			ref = std::make_shared<RCBlock>();
		}
		ref->sm.erase(moduleName);
		if (ref->sm.size() == 0) {
			it = this->list.erase(it);
			continue;
		}
		it++;
	}
}