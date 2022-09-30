#pragma once
#include <JuceHeader.h>

class StatusBarRoundButtonLAF final : public juce::LookAndFeel_V4
{
public:
	StatusBarRoundButtonLAF() :LookAndFeel_V4() {};
	~StatusBarRoundButtonLAF() override = default;

    void drawButtonBackground(juce::Graphics& g,
        juce::Button& button,
        const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        auto cornerSize = std::min(button.getWidth(), button.getHeight()) / 2.f;
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

        auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

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

    void drawDrawableButton(juce::Graphics& g, juce::DrawableButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        bool toggleState = button.getToggleState();

        auto cornerSize = std::min(button.getWidth(), button.getHeight()) / 2.f;
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

        auto baseColour = button.findColour(toggleState ? juce::DrawableButton::backgroundOnColourId
            : juce::DrawableButton::backgroundColourId).withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

        g.setColour(baseColour);
        g.fillRoundedRectangle(bounds, cornerSize);

        const int textH = (button.getStyle() == juce::DrawableButton::ImageAboveTextLabel)
            ? juce::jmin(16, button.proportionOfHeight(0.25f))
            : 0;

        if (textH > 0)
        {
            g.setFont((float)textH);

            g.setColour(button.findColour(toggleState ? juce::DrawableButton::textColourOnId
                : juce::DrawableButton::textColourId)
                .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.4f));

            g.drawFittedText(button.getButtonText(),
                2, button.getHeight() - textH - 1,
                button.getWidth() - 4, textH,
                juce::Justification::centred, 1);
        }
    }
};