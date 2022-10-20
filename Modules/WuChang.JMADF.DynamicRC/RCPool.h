#pragma once
#include <JuceHeader.h>

class RCPool final
{
public:
	RCPool() = default;
	~RCPool() = default;

	std::pair<size_t, void*> get(const juce::String& moduleName, const juce::String& path);
	std::pair<size_t, void*> copy(const juce::String& moduleName, const juce::String& path);
	void release(const juce::String& moduleName, const juce::String& path);
	
	void destory(const juce::String& moduleName);
	
private:
	class RCBlock final {
	public:
		RCBlock() = default;
		~RCBlock() = default;

		size_t size = 0;
		std::shared_ptr<char> ptr = nullptr;
		std::set<juce::String> sm;
		
	private:
		JUCE_LEAK_DETECTOR(RCBlock)
	};
	
	std::map<juce::String, std::shared_ptr<RCBlock>> list;
	juce::SpinLock rcLock;

	std::set<juce::String> mSet;
	juce::ReadWriteLock setLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RCPool)
};