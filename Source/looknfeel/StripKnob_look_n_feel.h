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

private:
    juce::Image mKnobStrip;
};