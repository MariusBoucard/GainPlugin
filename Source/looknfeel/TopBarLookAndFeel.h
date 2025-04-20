#pragma once 

class SettingsModalLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawComponentBackground(juce::Graphics& g, juce::Component& component)
    {
        auto bounds = component.getLocalBounds().toFloat();

        // Draw semi-transparent grey background
        g.setColour(juce::Colours::darkgrey.withAlpha(0.8f));
        g.fillRoundedRectangle(bounds, 15.0f);

        // Draw glowy green border
        g.setColour(juce::Colours::limegreen.withAlpha(0.8f));
        g.drawRoundedRectangle(bounds, 15.0f, 4.0f);
    }

    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        g.fillAll(juce::Colours::transparentBlack); // Transparent background for the label
        g.setColour(juce::Colours::white);         // White text
        g.setFont(juce::Font(20.0f, juce::Font::bold)); // Bold font with size 20
        g.drawText(label.getText(), label.getLocalBounds(), juce::Justification::centred, true);
    }
};