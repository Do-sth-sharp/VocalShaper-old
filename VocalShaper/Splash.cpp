#include "Splash.h"

Splash::Splash(const juce::String& version, const juce::String& cDateTime)
	:Component()
{
	this->setAlwaysOnTop(true);
	this->setMouseCursor(juce::MouseCursor::WaitCursor);
	this->setOpaque(false);

	this->closeTimer = std::make_unique<CloseTimer>(this);
	this->hideTimer = std::make_unique<HideTimer>(this);

	this->verStr = "Ver " + version + " Build " + cDateTime;
	
	this->logo = std::make_unique<juce::Image>(
		juce::ImageFileFormat::loadFrom(
			juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + "/rc/dark.png"
		));
}

void Splash::paint(juce::Graphics& g)
{
	//绘图片
	const double pixX = 0.5;
	const double pixY = 0.47;
	const double scale = 0.75;
	const double round = 0.01;
	
	juce::Rectangle<float> rect(
		(float)(this->getWidth() * pixX - this->getWidth() * scale / 2), 0.f,
		(float)(this->getWidth() * scale), (float)(this->getHeight() * pixY * 2)
	);
	
	g.setColour(juce::Colour(0xFF1B1B24));
	g.fillRoundedRectangle(this->getLocalBounds().toFloat(), (float)(this->getWidth() * round));
	g.drawImage(
		*(this->logo), rect,
		juce::RectanglePlacement(juce::RectanglePlacement::Flags::centred)
	);

	//绘文字
	const double labHei = 0.2;
	const double labSpl = 0.7;
	const double margin = 0.01;
	juce::Rectangle<int> mesRect(
		this->getWidth() * margin, this->getHeight() * (1 - labHei),
		this->getWidth() * labSpl - this->getWidth() * margin, this->getHeight() * labHei
	);
	juce::Rectangle<int> verRect(
		this->getWidth() - this->getWidth() * (1 - labSpl), this->getHeight() * (1 - labHei),
		this->getWidth() * (1 - labSpl) - this->getWidth() * margin, this->getHeight() * labHei
	);

	g.setColour(juce::Colour(0x9EFFFFFF));
	g.drawFittedText(this->mesStr, mesRect, juce::Justification::bottomLeft, 1);
	g.drawFittedText(this->verStr, verRect, juce::Justification::bottomRight, 1);
}

void Splash::mouseDown(const juce::MouseEvent& e)
{
	juce::Point<int> pos = e.getPosition();
	if (
		pos.getX() > 0 && pos.getY() > 0 &&
		pos.getX() < this->getWidth() && pos.getY() < this->getHeight()
		) {
		if (this->isReady) {
			this->closeSplash();
		}
	}
}

void Splash::ready()
{
	this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->isReady = true;
	this->closeTimer->startTimer(3000);
}

void Splash::showMessage(const juce::String& message)
{
	this->mesStr = message;
	this->repaint();
}

void Splash::closeSplash()
{
	if (this->isReady && this->isVisible() && !this->fader.isAnimating()) {
		constexpr int totalMillSec = 2000;
		this->fader.animateComponent(this, this->getBounds(), 0.0f, totalMillSec, false, 0, 0);
		this->hideTimer->startTimer(totalMillSec);
	}
}
