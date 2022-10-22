#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class ProjectHub final
{
public:
	ProjectHub() = default;
	~ProjectHub() = default;

	bool newProj(const juce::String& name, const juce::String& path);
	bool copyProj(const juce::String& name, const juce::String& path,
		const juce::String& nameSrc, const juce::String& pathSrc);
	bool openProj(const juce::String& name, const juce::String& path);

	void setCurrent(int index);
	void setCurrentAndToFront(int index);
	void close(int index);
	bool checkForClose(int index);
	vocalshaper::ProjectProxy* get(int index);
	int getCurrent();
	int getSize();
	bool save(int index);
	bool isOpened(const juce::String& name, const juce::String& path);
	bool isSaved(const juce::String& name, const juce::String& path);

	using ChangeNoticeFunction = std::function<void(const vocalshaper::ProjectProxy*)>;
	void addNotice(const juce::String& caller, const ChangeNoticeFunction& func);
	void release(const juce::String& caller);

private:
	juce::OwnedArray<vocalshaper::ProjectProxy> projList;
	int currentIndex = -1;

	juce::CriticalSection lock;

	std::map<juce::String, ChangeNoticeFunction> funcList;
	juce::ReadWriteLock funcLock;

	std::set<juce::String> mSet;
	juce::ReadWriteLock setLock;

	inline vocalshaper::ProjectProxy* create(const juce::String& name, const juce::String& path) const;
	inline int findOpened(const juce::String& name, const juce::String& path) const;
	void callNotice();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectHub)
};