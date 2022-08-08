#pragma once
#include <JuceHeader.h>

class LAFConfigs final
{
public:
	LAFConfigs() = default;
	~LAFConfigs() = default;
	
	bool getNum(const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, double& value);
	bool getString(const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, juce::String& value);
	bool getBool(const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, bool& value);
	bool getColour(const juce::String& caller, const juce::String& className, const juce::String& group, const juce::String& attribute, juce::Colour& value);
	
	void releaseAll();//�������ȫ���ͷ�
	void release(const juce::String& moduleName);//��������ͷ�ĳһģ�����µ�ȫ����
	
	void close(const juce::String& caller);//ĳһģ��رղ�����

private:
	class LAFFile final
	{
	public:
		juce::var data;
		std::set<juce::String> modules;
		bool flag = false;
		
	private:
		JUCE_LEAK_DETECTOR(LAFFile)
	};
	
private:
	enum class VarType
	{
		Number,
		Boolean,
		String
	};

	bool getDFS(const juce::String& caller, const juce::String& moduleName, const juce::String& className, const juce::String& group, const juce::String& attribute, VarType type, juce::var& value,
		std::set<std::pair<juce::String, juce::String>>& inheritList);
	bool load(const juce::String& caller, const juce::String& moduleName, const juce::String& className, juce::var*& data);
	bool getVar(const juce::String& caller, const juce::String& moduleName, const juce::String& className, const juce::String& variable, VarType type, juce::var& value,
		std::set<std::pair<juce::String, juce::String>>& inheritList, std::set<juce::String>& refList);

	inline uint8_t HexToDec(const juce::juce_wchar& s) const;
private:
	std::map<std::pair<juce::String, juce::String>, LAFFile> list;
	juce::SpinLock lock;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LAFConfigs)
};