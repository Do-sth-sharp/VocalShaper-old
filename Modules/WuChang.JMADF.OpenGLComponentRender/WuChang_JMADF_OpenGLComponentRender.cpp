#include "WuChang_JMADF_OpenGLComponentRender.h"

bool WuChang_JMADF_OpenGLComponentRender::init()
{
	if (!jmadf::LoadModule("WuChang.JMADF.Configs")) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.Configs", "GetReference")
		) {
		jmadf::RaiseException("@WuChang.JMADF.Configs:Bad Interfaces!");
		return false;
	}
	juce::var* config = nullptr;
	bool ok = false;
	jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
		"WuChang.JMADF.Configs", "GetReference",
		"config", config, ok
		);
	if (ok && (config != nullptr)) {
		if ((*config)["OpenGLOn"].isBool()) {
			this->OpenGLOn = (*config)["OpenGLOn"];
		}
	}

	this->glContext = std::make_unique<juce::OpenGLContext>();
	jmadf::RegisterInterface<juce::Component*>(
		"Attach",
		[this](const juce::String&, juce::Component* comp) {
			if (this->OpenGLOn) {
				if (!this->glContext->isAttached()) {
					this->glContext->attachTo(*comp);
				}
			}
		}
	);
	jmadf::RegisterInterface<void>(
		"Detach",
		[this](const juce::String&) {
			if (this->OpenGLOn) {
				if (this->glContext->isAttached()) {
					this->glContext->detach();
				}
			}
		}
	);
	return this->OpenGLOn;
}

void WuChang_JMADF_OpenGLComponentRender::destory()
{
	if (this->glContext) {
		this->glContext->detach();
	}
	this->glContext = nullptr;
}
