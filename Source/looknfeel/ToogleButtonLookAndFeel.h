#pragma once
#include <JuceHeader.h>
class ToggleButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ToggleButtonLookAndFeel() {}

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();

        juce::Colour bgColour = juce::Colours::transparentWhite;
        if (isMouseOverButton)
        {
            bgColour = bgColour.withAlpha(0.4f);
        }
        if (isButtonDown)
        {
            bgColour = bgColour.withAlpha(0.6f);
        }

        g.setColour(bgColour);
        g.fillRoundedRectangle(bounds, 6.0f);

        g.setColour(juce::Colours::limegreen);
        g.drawRoundedRectangle(bounds, 6.0f, 2.0f);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        // Set the text color based on hover state
        juce::Colour textColour = juce::Colours::white;
        if (isMouseOverButton)
        {
            textColour = juce::Colours::black; // Change text color to black when hovered
        }

        g.setColour(textColour);

        // Draw the text centered in the button
        g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);
    }
};
