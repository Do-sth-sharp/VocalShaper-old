#include "WuChang_JMADF_OpenGLComponentRender.h"

bool WuChang_JMADF_OpenGLComponentRender::init()
{
	this->glContext = std::make_unique<juce::OpenGLContext>();
	jmadf::RegisterInterface<juce::Component*>(
		"Attach",
		[this](const juce::String&, juce::Component* comp) {
			if (!this->glContext->isAttached()) {
				this->glContext->attachTo(*comp);
			}
		}
	);
	jmadf::RegisterInterface<void>(
		"Detach",
		[this](const juce::String&) {
			if (this->glContext->isAttached()) {
				this->glContext->detach();
			}
		}
	);
	return true;
}

void WuChang_JMADF_OpenGLComponentRender::destory()
{
	if (this->glContext->isAttached()) {
		this->glContext->detach();
	}
	this->glContext = nullptr;
}
