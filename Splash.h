#pragma once
#include <JuceHeader.h>

class Splash final :
    public juce::Component
{
public:
	Splash(const juce::String& version, const juce::String& cDateTime);
	~Splash() = default;
	
	void resized() override;
	void paint(juce::Graphics& g) override;
	void mouseDown(const juce::MouseEvent& e) override;
	
	void ready();
	void showMessage(const juce::String& message);
	
private:
	bool isReady = false;
	std::unique_ptr<juce::Label> mesLabel, verLabel;
	std::unique_ptr<juce::Image> logo;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Splash)
};

