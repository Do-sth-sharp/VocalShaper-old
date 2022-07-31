#include "SMComponent.h"
#include <libJModule.h>

SMComponent::SMComponent()
	: Component("Start Menu Component")
{
    bool result = false;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background", this->colors.background, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "leftBar", this->colors.leftBar, result
        );

    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-leftBar", this->sizes.width_leftBar, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-leftBar-max", this->sizes.width_leftBar_max, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "position", "posY-logo", this->positions.posY_logo, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "width-logo", this->scales.width_logo, result
        );

    juce::String logoFile;
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::String&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetString",
        "main", "resource", "logo", logoFile, result
        );
	
    size_t logoSize = 0;
    void* logoPtr = nullptr;
    juce::String logoPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
        .getParentDirectory().getFullPathName() + logoFile;

    jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
        "WuChang.JMADF.DynamicRC",
        "GetRC",
        logoPath, std::pair<size_t&, void*&>(logoSize, logoPtr)
        );

    this->logoImage = juce::ImageFileFormat::loadFrom(logoPtr, logoSize);
}

SMComponent::~SMComponent()
{
	
}

void SMComponent::paint(juce::Graphics& g)
{
    juce::Rectangle<int> screenSize;
    double screenScale = 1;
    jmadf::CallInterface<juce::Component*, juce::Rectangle<int>&>(
        "WuChang.JMADF.Device", "GetScreenSize",
        this, screenSize
        );
    /*jmadf::CallInterface<juce::Component*, double&>(
        "WuChang.JMADF.Device", "GetScreenScale",
        this, screenScale
        );*/
	
    g.fillAll(this->colors.background);
	
    int leftBarWidth =
        (int)std::min(this->sizes.width_leftBar * screenSize.getWidth() * screenScale, this->sizes.width_leftBar_max);
	
    g.setColour(this->colors.leftBar);
    g.fillRect(0, 0, leftBarWidth, this->getHeight());

    double logoScale = (leftBarWidth * this->scales.width_logo) / (double)(this->logoImage.getWidth());
    juce::Rectangle<int> logoRect(
        (leftBarWidth / 2.0) * (1 - this->scales.width_logo), (screenSize.getHeight() * this->positions.posY_logo) - (this->logoImage.getHeight() * logoScale / 2),
        leftBarWidth * this->scales.width_logo, this->logoImage.getHeight() * logoScale
    );
    g.drawImageWithin(
        this->logoImage,
        logoRect.getX(), logoRect.getY(),
        logoRect.getWidth(), logoRect.getHeight(),
        juce::RectanglePlacement::Flags::centred
    );
}
