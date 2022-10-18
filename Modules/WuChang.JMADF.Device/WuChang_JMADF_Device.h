#pragma once
#include <libJModule.h>

class WuChang_JMADF_Device final :
	public jmadf::Module
{
public:
	WuChang_JMADF_Device() = default;
	~WuChang_JMADF_Device() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.Device"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };
	
public:
	static const juce::Rectangle<int> getScreenSize(juce::Component* component);
	static const double getScreenScale(juce::Component* component);
	static const juce::Displays::Display* getCurrentScreen(juce::Component* component);
	
public:
	const juce::Displays::Display* ptrCurrentDisplay = nullptr;
	double currentScale = 1.0;
	juce::Rectangle<int> sizeTemp;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WuChang_JMADF_Device)
};