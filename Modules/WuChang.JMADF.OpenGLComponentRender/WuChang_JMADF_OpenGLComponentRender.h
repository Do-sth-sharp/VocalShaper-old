#pragma once
#include <libJModule.h>

class WuChang_JMADF_OpenGLComponentRender final :
	public jmadf::Module
{
public:
	WuChang_JMADF_OpenGLComponentRender() = default;
	~WuChang_JMADF_OpenGLComponentRender() = default;

	bool init() override;
	void destory() override;

	const juce::String getModuleName() override { return "WuChang.JMADF.OpenGLComponentRender"; };
	const juce::String getModuleVersion() override { return "0.0.1"; };

	std::unique_ptr<juce::OpenGLContext> glContext;
	bool OpenGLOn = true;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WuChang_JMADF_OpenGLComponentRender)
};