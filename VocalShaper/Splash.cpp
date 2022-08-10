#include "Splash.h"

Splash::Splash(const juce::String& version, const juce::String& cDateTime)
	:Component()
{
	this->setAlwaysOnTop(true);
	this->setMouseCursor(juce::MouseCursor::WaitCursor);
	//this->setOpaque(false);

	this->mesLabel = std::make_unique<juce::Label>();
	this->mesLabel->setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(0x0));
	this->mesLabel->setColour(juce::Label::ColourIds::textColourId, juce::Colour(0x9EFFFFFF));
	this->mesLabel->setJustificationType(juce::Justification::bottomLeft);
	this->mesLabel->setAlwaysOnTop(true);
	this->addAndMakeVisible(this->mesLabel.get());

	this->verLabel = std::make_unique<juce::Label>("", "Ver " + version + " Build " + cDateTime);
	this->verLabel->setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(0x0));
	this->verLabel->setColour(juce::Label::ColourIds::textColourId, juce::Colour(0x9EFFFFFF));
	this->verLabel->setJustificationType(juce::Justification::bottomRight);
	this->verLabel->setAlwaysOnTop(true);
	this->addAndMakeVisible(this->verLabel.get());

	this->mesLabel->setMouseCursor(juce::MouseCursor::WaitCursor);
	this->verLabel->setMouseCursor(juce::MouseCursor::WaitCursor);
	
	this->logo = std::make_unique<juce::Image>(
		juce::ImageFileFormat::loadFrom(
			juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
			.getParentDirectory().getFullPathName() + "/rc/dark.png"
		));
}

void Splash::resized()
{
	const double labHei = 0.2;
	const double labSpl = 0.7;
	
	this->mesLabel->setSize((int)(this->getWidth() * labSpl), (int)(this->getHeight() * labHei));
	this->verLabel->setSize((int)(this->getWidth() * (1 - labSpl)), (int)(this->getHeight() * labHei));
	
	this->mesLabel->setTopLeftPosition(0, (int)(this->getHeight() * (1 - labHei)));
	this->verLabel->setTopRightPosition(this->getWidth(), (int)(this->getHeight() * (1 - labHei)));
}

void Splash::paint(juce::Graphics& g)
{
	const double pixX = 0.5;
	const double pixY = 0.47;
	const double scale = 0.75;
	const double round = 0.01;
	
	juce::Rectangle<float> rect(
		(float)(this->getWidth() * pixX - this->getWidth() * scale / 2), 0.f,
		(float)(this->getWidth() * scale), (float)(this->getHeight() * pixY * 2)
	);
	
	g.fillAll(juce::Colour(0x0));
	g.setColour(juce::Colour(0xFF1B1B24));
	g.fillRoundedRectangle(this->getLocalBounds().toFloat(), (float)(this->getWidth() * round));
	g.drawImage(
		*(this->logo), rect,
		juce::RectanglePlacement(juce::RectanglePlacement::Flags::centred)
	);
}

void Splash::mouseDown(const juce::MouseEvent& e)
{
	juce::Point<int> pos = e.getPosition();
	if (
		pos.getX() > 0 && pos.getY() > 0 &&
		pos.getX() < this->getWidth() && pos.getY() < this->getHeight()
		) {
		if (this->isReady) {
			this->setVisible(false);
		}
	}
}

void Splash::ready()
{
	this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->mesLabel->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->verLabel->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->isReady = true;
}

void Splash::showMessage(const juce::String& message)
{
	this->mesLabel->setText(message, juce::NotificationType::dontSendNotification);
}
