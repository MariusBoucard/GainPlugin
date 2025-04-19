#include "gui.h"

#include "AmpAudioProcessor.h"
#include <JuceHeader.h>
int RootViewComponent::ROOT_WIDTH = 980;
int RootViewComponent::ROOT_HEIGHT = 550;

RootViewComponent::RootViewComponent(juce::AudioProcessor& processor)
    : AudioProcessorEditor(processor)
    , mInputMeter(processor)
    , mOutputMeter(processor,false)
    , mFileChooser("choose NAM file")

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

    mFileChooserButton.setButtonText("Load IR :");
    mFileChooserButton.onClick = [this]() { openFileChooser(); };
    mNAMChooserButton.setButtonText("Load NAM :");
    mNAMChooserButton.onClick = [this]() { openNAMFileChooser(); };

    mInputKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mBassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mMidKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mHighKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mOutputKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mGateKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mVerbMixKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);

    mIRButton.setButtonText("IR");
    mNAMButton.setButtonText("NAM");
    mIRVerbButton.setButtonText("IR Verb");

    mInputKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mInputKnobLayout.inLayout.textboxHeight);
    mGateKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mGateKnobLayout.inLayout.textboxHeight);
    mBassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mBassKnobLayout.inLayout.textboxHeight);
    mMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mMidKnobLayout.inLayout.textboxHeight);
    mHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mHighKnobLayout.inLayout.textboxHeight);
    mOutputKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mOutputKnobLayout.inLayout.textboxHeight);
    mVerbMixKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mVerbMixKnobLayout.inLayout.textboxHeight);
    
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
    mIRButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		ampAudioProcessor->getCustomParameterTree(), "irEnabled", mIRButton);
    mNAMButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            ampAudioProcessor->getCustomParameterTree(), "namEnabled", mNAMButton);
    mIRVerbButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
             ampAudioProcessor->getCustomParameterTree(), "irVerbEnabled", mIRVerbButton);
    mVerbMixKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		ampAudioProcessor->getCustomParameterTree(), "irVerbMix", mVerbMixKnob);
	
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

	mIRButton.setBounds(mIRButtonLayout.outLayout.x,
						mIRButtonLayout.outLayout.y,
						mIRButtonLayout.outLayout.sliderWidth,
						mIRButtonLayout.outLayout.sliderHeight);

    mFileChooserButton.setBounds(mFileChooserButtonLayout.outLayout.x,
                                mFileChooserButtonLayout.outLayout.y,
                                mFileChooserButtonLayout.outLayout.sliderWidth,
                                mFileChooserButtonLayout.outLayout.sliderHeight);
    mNAMChooserButton.setBounds(mNAMChooserButtonLayout.outLayout.x,
                                mNAMChooserButtonLayout.outLayout.y,
                                mNAMChooserButtonLayout.outLayout.sliderWidth,
                                mNAMChooserButtonLayout.outLayout.sliderHeight);
    mNAMButton.setBounds(mNAMButtonLayout.outLayout.x,
							mNAMButtonLayout.outLayout.y,
							mNAMButtonLayout.outLayout.sliderWidth,
							mNAMButtonLayout.outLayout.sliderHeight);
    mIRVerbButton.setBounds(mIRVerbButtonLayout.outLayout.x,
							mIRVerbButtonLayout.outLayout.y,
							mIRVerbButtonLayout.outLayout.sliderWidth,
							mIRVerbButtonLayout.outLayout.sliderHeight);
    mVerbMixKnob.setBounds(mVerbMixKnobLayout.outLayout.x,
							mVerbMixKnobLayout.outLayout.y,
							mVerbMixKnobLayout.outLayout.sliderWidth,
							mVerbMixKnobLayout.outLayout.sliderHeight);


    addAndMakeVisible(mInputMeter);
    addAndMakeVisible(mInputKnob);
    addAndMakeVisible(mGateKnob);
    addAndMakeVisible(mBassKnob);
    addAndMakeVisible(mMidKnob);
    addAndMakeVisible(mHighKnob);
    addAndMakeVisible(mOutputKnob);
    addAndMakeVisible(mOutputMeter);
    addAndMakeVisible(mIRButton);
    addAndMakeVisible(mFileChooserButton);
    addAndMakeVisible(mNAMChooserButton);
    addAndMakeVisible(mNAMButton);
    addAndMakeVisible(mIRVerbButton);
    addAndMakeVisible(mVerbMixKnob);
   

    mInputKnob.setLookAndFeel(&mKnobLookAndFeel);
    mGateKnob.setLookAndFeel(&mKnobLookAndFeel);
    mBassKnob.setLookAndFeel(&mKnobLookAndFeel);
    mMidKnob.setLookAndFeel(&mKnobLookAndFeel);
    mHighKnob.setLookAndFeel(&mKnobLookAndFeel);
    mOutputKnob.setLookAndFeel(&mKnobLookAndFeel);
    mVerbMixKnob.setLookAndFeel(&mKnobLookAndFeel);
    mFileChooserButton.setLookAndFeel(&mToggleButtonLookAndFeel);
    mNAMChooserButton.setLookAndFeel(&mToggleButtonLookAndFeel);

    mInputKnob.setValue(0.5);
    mGateKnob.setValue(0.5);
    mBassKnob.setValue(0.5);
    mMidKnob.setValue(0.5);
    mHighKnob.setValue(0.5);
    mOutputKnob.setValue(0.5);
    mVerbMixKnob.setValue(0.5);
}

void RootViewComponent::handleSelectedFile(const juce::File& file)
{
    DBG("Handling file: " << file.getFullPathName());
    AmpAudioProcessor& audioProcessor = static_cast<AmpAudioProcessor&>(processor);
    audioProcessor.loadImpulseResponse(file);
    std::string filePath = file.getFullPathName().toStdString().substr(file.getFullPathName().length() - 20, file.getFullPathName().length());
    mFileChooserButton.setButtonText("Load IR : "+ filePath);

}
void RootViewComponent::openFileChooser()
{
    juce::FileChooser chooser("Select an Impulse Response File",
        juce::File{},
        "*.wav");

    if (chooser.browseForFileToOpen())
    {
        handleSelectedFile(chooser.getResult());
    }
}

void RootViewComponent::openNAMFileChooser()
{
    juce::FileChooser chooser("Select NAM File",
        juce::File{},
        "*.nam");

    if (chooser.browseForFileToOpen())
    {
        handleSelectedNAMFile(chooser.getResult());
    }
}
void RootViewComponent::handleSelectedNAMFile(const juce::File& file)
{
	DBG("Handling NAM file: " << file.getFullPathName());
	AmpAudioProcessor& audioProcessor = static_cast<AmpAudioProcessor&>(processor);
	audioProcessor.loadNAMFile(file);
    std::string filePath = file.getFullPathName().toStdString().substr(file.getFullPathName().length() - 20, file.getFullPathName().length());
    mNAMChooserButton.setButtonText("Load NAM : "+ filePath);
}
void RootViewComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    if (!mImage.isNull())
    {
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

	    g.setColour(juce::Colours::blue);
}