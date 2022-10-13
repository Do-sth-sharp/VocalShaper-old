#pragma once
#include <JuceHeader.h>

class Splash final : public juce::Component
{
public:
	Splash(const juce::String& version, const juce::String& cDateTime);
	~Splash() override = default;
	
	void resized() override;
	void paint(juce::Graphics& g) override;
	void mouseDown(const juce::MouseEvent& e) override;
	
	void ready();
	void showMessage(const juce::String& message);

	void closeSplash();
	
private:
	bool isReady = false;
	std::unique_ptr<juce::Label> mesLabel, verLabel;
	std::unique_ptr<juce::Image> logo;

	class CloseTimer final : public juce::Timer
	{
		Splash* splash = nullptr;
	public:
		CloseTimer(Splash* splash) :Timer(), splash(splash) {};
		void timerCallback() override
		{
			if (this->splash) {
				this->splash->closeSplash();
			}
		};
	};
	class HideTimer final : public juce::Timer
	{
		Splash* splash = nullptr;
	public:
		HideTimer(Splash* splash) :Timer(), splash(splash) {};
		void timerCallback() override
		{
			if (this->splash && this->splash->isVisible()) {
				this->splash->setVisible(false);
			}
		};
	};
	std::unique_ptr<CloseTimer> closeTimer;
	std::unique_ptr<HideTimer> hideTimer;
	juce::ComponentAnimator fader;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Splash)
};

