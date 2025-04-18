#include "gui.h"
#include "AmpAudioProcessor.h"
#include <JuceHeader.h>
int RootViewComponent::ROOT_WIDTH = 1000;
int RootViewComponent::ROOT_HEIGHT = 550;

RootViewComponent::RootViewComponent(juce::AudioProcessor& processor)
    : AudioProcessorEditor(processor) // Correct initialization
{
    auto& gainProcessor = processor; // Cast to GainProcessor

    // Load the image from BinaryData
    auto imageData = BinaryData::plate_png; // Replace 'plate_png' with the actual name of your resource
    auto imageDataSize = BinaryData::plate_pngSize; // Replace 'plate_pngSize' with the actual size of your resource

    mImage = juce::ImageFileFormat::loadFrom(imageData, imageDataSize);

    if (mImage.isNull())
        DBG("Failed to load image from resources");

    // Set the size of the editor
    setSize(ROOT_WIDTH,ROOT_HEIGHT);
    configureNodes(gainProcessor);
}

void RootViewComponent::configureNodes(juce::AudioProcessor& gainProcessor)
{

    int textboxHeight = 20; 
    mGainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mGainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, textboxHeight);
    
    auto bounds = getLocalBounds();
    int frameWidth = 135;  
    int frameHeight = 153;
    float ratio = 0.5f;

    int sliderWidth = frameWidth * ratio;
    int sliderHeight = ( frameHeight + textboxHeight + 10 ) * ratio;
    AmpAudioProcessor* gainProcessorG = dynamic_cast<AmpAudioProcessor*>(&gainProcessor);

    mGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        gainProcessorG->getCustomParameterTree(), "input", mGainKnob);

    mGainKnob.setBounds(180,
        235,
        sliderWidth,
        sliderHeight);

    addAndMakeVisible(mGainKnob);
    mGainKnob.setLookAndFeel(&mKnobLookAndFeel);
	mGainKnob.setValue(0.5);
}

void RootViewComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black); // Fill background with black

    if (!mImage.isNull())
    {
        // Draw the image centered in the component
        auto bounds = getLocalBounds().toFloat();
        auto imageBounds = mImage.getBounds().toFloat();
        auto scale = juce::jmin(bounds.getWidth() / imageBounds.getWidth(),
            bounds.getHeight() / imageBounds.getHeight());
        auto scaledImageBounds = imageBounds.withSize(imageBounds.getWidth() * scale,
            imageBounds.getHeight() * scale);
        g.drawImage(mImage, scaledImageBounds);
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.drawText("Image not found", getLocalBounds(), juce::Justification::centred);
    }
}

void RootViewComponent::resized()
{
    auto bounds = getLocalBounds();

    mGainKnob.setBounds(bounds.getCentreX() - 50, bounds.getCentreY() - 50, 100, 100); // 100x100 knob
    // Handle resizing logic if needed
}