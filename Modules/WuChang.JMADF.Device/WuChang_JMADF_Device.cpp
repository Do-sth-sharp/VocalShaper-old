#include "WuChang_JMADF_Device.h"

bool WuChang_JMADF_Device::init()
{
	jmadf::RegisterInterface<juce::Component*, juce::Rectangle<int>&>(
		"GetScreenSize",
		[](const juce::String&, juce::Component* component, juce::Rectangle<int>& result) {
			result = WuChang_JMADF_Device::getScreenSize(component);
		}
	);
	jmadf::RegisterInterface<juce::Component*, double&>(
		"GetScreenScale",
		[](const juce::String&, juce::Component* component, double& result) {
			result = WuChang_JMADF_Device::getScreenScale(component);
		}
	);
	return true;
}

void WuChang_JMADF_Device::destory()
{
}

const juce::Rectangle<int> WuChang_JMADF_Device::getScreenSize(juce::Component* component)
{
	const juce::Displays::Display* display = WuChang_JMADF_Device::getCurrentScreen(component);
	if (display == nullptr) {
		return juce::Rectangle<int>();
	}
	return display->userArea;
}

const double WuChang_JMADF_Device::getScreenScale(juce::Component* component)
{
	const juce::Displays::Display* display = WuChang_JMADF_Device::getCurrentScreen(component);
	if (display == nullptr) {
		return 1;
	}
	return display->scale;
}

const juce::Displays::Display* WuChang_JMADF_Device::getCurrentScreen(juce::Component* component)
{
	return juce::Desktop::getInstance().getDisplays().getDisplayForRect(component->getScreenBounds());
}
