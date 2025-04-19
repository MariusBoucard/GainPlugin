#pragma once

#include <JuceHeader.h>

class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    KnobLookAndFeel()
    {
        // Load the strip image from BinaryData
        auto imageData = BinaryData::knob_strip_png; // Replace with your strip image name
        auto imageDataSize = BinaryData::knob_strip_pngSize; // Replace with your strip image size
        mKnobStrip = juce::ImageFileFormat::loadFrom(imageData, imageDataSize);

        if (mKnobStrip.isNull())
            DBG("Failed to load knob strip image from resources");
    }


    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override
    {
        if (mKnobStrip.isNull())
            return;

        const int numFrames = 260;
        int column = 5;
        const int row = numFrames / column; // Assuming the first row of the image
        const int frameIndex = (int)std::round(sliderPosProportional * (numFrames - 1)) % numFrames;
        int frame_width = 135;
        int frame_height = 153;

        int line = frameIndex / column;
        int columnIndex = frameIndex % column;
        const auto sourceRect = juce::Rectangle<int>(columnIndex*frame_width, line*frame_height, frame_width, frame_height);

        g.drawImage(mKnobStrip, x, y, width, height, sourceRect.getX(), sourceRect.getY(),
                    sourceRect.getWidth(), sourceRect.getHeight());
    }


    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        if (auto* slider = dynamic_cast<juce::Slider*>(label.getParentComponent()))
        {
            if (!slider->isMouseOverOrDragging())
            {
                // If not hovered, don't draw the label
                return;
            }
        }
        juce::Rectangle bounds = label.getLocalBounds();
        bounds.setX(15);
        bounds.setWidth(bounds.getWidth() -15);

        g.setColour(juce::Colours::darkgrey);
        g.fillRect(bounds);
        g.setColour(juce::Colours::limegreen);
        g.drawRect(bounds, 2.0f); // Border thickness of 2.0f


      

        // Draw the text
        g.setColour(juce::Colours::white);
        g.setFont(label.getFont());
        g.drawText(label.getText(), bounds, juce::Justification::centred, true);
    }
private:
    juce::Image mKnobStrip;
};