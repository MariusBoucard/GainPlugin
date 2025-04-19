#include "gui.h"

#include "AmpAudioProcessor.h"
#include <JuceHeader.h>
int RootViewComponent::ROOT_WIDTH = 1000;
int RootViewComponent::ROOT_HEIGHT = 550;

RootViewComponent::RootViewComponent(juce::AudioProcessor& processor)
    : AudioProcessorEditor(processor)
    , mInputMeter(processor)
    , mOutputMeter(processor,false)
{
    auto& gainProcessor = processor; 

    auto imageData = BinaryData::plate_png;
    auto imageDataSize = BinaryData::plate_pngSize;

    mImage = juce::ImageFileFormat::loadFrom(imageData, imageDataSize);

    if (mImage.isNull())
        DBG("Failed to load image from resources");


    setSize(ROOT_WIDTH,ROOT_HEIGHT);

    defineKnobLayout();

    configureNodes(gainProcessor);
}

void RootViewComponent::configureNodes(juce::AudioProcessor& inProcessor)
{
    auto bounds = getLocalBounds();

    AmpAudioProcessor* ampAudioProcessor = dynamic_cast<AmpAudioProcessor*>(&inProcessor);

    mInputKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mBassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mMidKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mHighKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mOutputKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mGateKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
   

    mInputKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, mInputKnobLayout.inLayout.textboxHeight);
    mGateKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, mGateKnobLayout.inLayout.textboxHeight);
    mBassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, mBassKnobLayout.inLayout.textboxHeight);
    mMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, mMidKnobLayout.inLayout.textboxHeight);
    mHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, mHighKnobLayout.inLayout.textboxHeight);
    mOutputKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, mOutputKnobLayout.inLayout.textboxHeight);

    mInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        ampAudioProcessor->getCustomParameterTree(), "input", mInputKnob);
    mBassKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		ampAudioProcessor->getCustomParameterTree(), "bass", mBassKnob);
    mMidKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        ampAudioProcessor->getCustomParameterTree(), "mid", mMidKnob);
    mHighKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        ampAudioProcessor->getCustomParameterTree(), "high", mHighKnob);
    mOutputKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        ampAudioProcessor->getCustomParameterTree(), "output", mOutputKnob);
    mGateKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    ampAudioProcessor->getCustomParameterTree(), "gate", mGateKnob);


    mInputKnob.setBounds(mInputKnobLayout.outLayout.x,
                         mInputKnobLayout.outLayout.y,
                         mInputKnobLayout.outLayout.sliderWidth,
                         mInputKnobLayout.outLayout.sliderHeight);
mGateKnob.setBounds(mGateKnobLayout.outLayout.x,
                    mGateKnobLayout.outLayout.y,
						mGateKnobLayout.outLayout.sliderWidth,
						mGateKnobLayout.outLayout.sliderHeight);
    mBassKnob.setBounds(mBassKnobLayout.outLayout.x,
                        mBassKnobLayout.outLayout.y,
        		        mBassKnobLayout.outLayout.sliderWidth,
        		        mBassKnobLayout.outLayout.sliderHeight);
    mMidKnob.setBounds(mMidKnobLayout.outLayout.x,
        					   mMidKnobLayout.outLayout.y,
        					   mMidKnobLayout.outLayout.sliderWidth,
        					   mMidKnobLayout.outLayout.sliderHeight);
    mHighKnob.setBounds(mHighKnobLayout.outLayout.x,
                        mHighKnobLayout.outLayout.y,
                        mHighKnobLayout.outLayout.sliderWidth,
                        mHighKnobLayout.outLayout.sliderHeight);
    mOutputKnob.setBounds(mOutputKnobLayout.outLayout.x,
        				  mOutputKnobLayout.outLayout.y,
        				  mOutputKnobLayout.outLayout.sliderWidth,
        				  mOutputKnobLayout.outLayout.sliderHeight);
mInputMeter.setBounds(mInputMeterLayout.outLayout.x,
						  mInputMeterLayout.outLayout.y,
						  mInputMeterLayout.outLayout.sliderWidth,
						  mInputMeterLayout.outLayout.sliderHeight);
mOutputMeter.setBounds(mOutputMeterLayout.outLayout.x,
						  mOutputMeterLayout.outLayout.y,
						  mOutputMeterLayout.outLayout.sliderWidth,
						  mOutputMeterLayout.outLayout.sliderHeight);


    addAndMakeVisible(mInputMeter);
    addAndMakeVisible(mInputKnob);
    addAndMakeVisible(mGateKnob);
    addAndMakeVisible(mBassKnob);
    addAndMakeVisible(mMidKnob);
    addAndMakeVisible(mHighKnob);
    addAndMakeVisible(mOutputKnob);
    addAndMakeVisible(mOutputMeter);
   

    mInputKnob.setLookAndFeel(&mKnobLookAndFeel);
    mGateKnob.setLookAndFeel(&mKnobLookAndFeel);
    mBassKnob.setLookAndFeel(&mKnobLookAndFeel);
    mMidKnob.setLookAndFeel(&mKnobLookAndFeel);
    mHighKnob.setLookAndFeel(&mKnobLookAndFeel);
    mOutputKnob.setLookAndFeel(&mKnobLookAndFeel);

    mInputKnob.setValue(0.5);
    mGateKnob.setValue(0.5);
    mBassKnob.setValue(0.5);
    mMidKnob.setValue(0.5);
    mHighKnob.setValue(0.5);
    mOutputKnob.setValue(0.5);
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

    //mInputKnob.setBounds(bounds.getCentreX() - 50, bounds.getCentreY() - 50, 100, 100); // 100x100 knob
    // Handle resizing logic if needed
}

void RootViewComponent::MeterComponent::paint(juce::Graphics& g)
{
    

        AmpAudioProcessor& meter = static_cast<AmpAudioProcessor&>(audioProcessor);
        float rmsLeft = 0.0f;
        float rmsRight = 0.0f;

        if (isInput)
        {
            rmsLeft = meter.getRmsLevelLeft();
            rmsRight = meter.getRmsLevelRight();
            g.setColour(juce::Colours::white);
            rmsLeft = rmsLeft * 10;

        }
        else
        {
			rmsLeft = meter.getRmsOutputLevelLeft();
			rmsRight = meter.getRmsOutputLevelRight();
            g.setColour(juce::Colours::green);
            rmsLeft = rmsLeft*10;

        }

        juce::Rectangle<float> meterBounds(0, 0, getWidth(), getHeight());
        juce::Rectangle<float> leftMeterBounds(0, getHeight() - (rmsLeft * getHeight()), getWidth() / 2, rmsLeft * getHeight());
        g.fillRect(juce::Rectangle<float>(10, 10, 3, rmsLeft * getHeight()));
      //  g.setColour(juce::Colours::blue);

   //     g.fillRect(juce::Rectangle<float>(50, getHeight() - (rmsLeft * getHeight()), 20, rmsLeft * getHeight()));

    

	// Draw the meter
	g.setColour(juce::Colours::blue);
	//g.fillRect(getLocalBounds().toFloat().reduced(10)); // Draw a rectangle for the meter
}