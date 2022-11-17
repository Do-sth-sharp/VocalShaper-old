#pragma once
#include <JuceHeader.h>

class TrackViewLinkButtonLAF final : public juce::LookAndFeel_V4
{
public:
    TrackViewLinkButtonLAF() :LookAndFeel_V4() {};
    ~TrackViewLinkButtonLAF() override = default;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto cornerSize = 6.0f;
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

        auto baseColour = backgroundColour.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

        if (shouldDrawButtonAsDown)
            baseColour = baseColour.contrasting(0.025f);

        g.setColour(baseColour);

        auto flatOnLeft = button.isConnectedOnLeft();
        auto flatOnRight = button.isConnectedOnRight();
        auto flatOnTop = button.isConnectedOnTop();
        auto flatOnBottom = button.isConnectedOnBottom();

        if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
        {
            juce::Path path;
            path.addRoundedRectangle(bounds.getX(), bounds.getY(),
                bounds.getWidth(), bounds.getHeight(),
                cornerSize, cornerSize,
                !(flatOnLeft || flatOnTop),
                !(flatOnRight || flatOnTop),
                !(flatOnLeft || flatOnBottom),
                !(flatOnRight || flatOnBottom));

            g.fillPath(path);

            g.setColour(button.findColour(juce::ComboBox::outlineColourId));
            g.strokePath(path, juce::PathStrokeType(1.0f));
        }
        else
        {
            g.fillRoundedRectangle(bounds, cornerSize);

            g.setColour(button.findColour(juce::ComboBox::outlineColourId));
            g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
        }
    }
};