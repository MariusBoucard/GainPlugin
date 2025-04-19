#pragma once
#include "looknfeel/StripKnob_look_n_feel.h"
#include <JuceHeader.h>

class RootViewComponent : public juce::AudioProcessorEditor
{
public:
    struct KnobLayout {
        struct input {
            int x;
            int y;
            int frameWidth ;
            int frameHeight;
            float ratio;
            int textboxHeight;
            int textboxPadding;
        };
        struct output {
            int x;
            int y;
            int sliderWidth;
            int sliderHeight;
        };
        input inLayout;
        output outLayout;
    };

    static int ROOT_WIDTH;
    static int ROOT_HEIGHT;
    RootViewComponent(juce::AudioProcessor& processor);

    void defineKnobLayout()
    {


        mInputKnobLayout.inLayout.x = 180;
		mInputKnobLayout.inLayout.y = 235;
		mInputKnobLayout.inLayout.frameWidth = 135;
		mInputKnobLayout.inLayout.frameHeight = 153;
		mInputKnobLayout.inLayout.ratio = 0.5f;
		mInputKnobLayout.inLayout.textboxHeight = 20;
        mInputKnobLayout.inLayout.textboxPadding = 10;

        mGateKnobLayout.inLayout.x = 260;
		mGateKnobLayout.inLayout.y = 235;
        mGateKnobLayout.inLayout.frameWidth = 135;
		mGateKnobLayout.inLayout.frameHeight = 153;
        mGateKnobLayout.inLayout.ratio = 0.5f;
		mGateKnobLayout.inLayout.textboxHeight = 20;
        mGateKnobLayout.inLayout.textboxPadding = 10;

        mBassKnobLayout.inLayout.x = 390;
        mBassKnobLayout.inLayout.y = 235;
        mBassKnobLayout.inLayout.frameWidth = 135;
        mBassKnobLayout.inLayout.frameHeight = 153;
        mBassKnobLayout.inLayout.ratio = 0.5f;
        mBassKnobLayout.inLayout.textboxHeight = 20;
        mBassKnobLayout.inLayout.textboxPadding = 10;

        mMidKnobLayout.inLayout.x = 460;
        mMidKnobLayout.inLayout.y = 235;
        mMidKnobLayout.inLayout.frameWidth = 135;
        mMidKnobLayout.inLayout.frameHeight = 153;
        mMidKnobLayout.inLayout.ratio = 0.5f;
        mMidKnobLayout.inLayout.textboxHeight = 20;
        mMidKnobLayout.inLayout.textboxPadding = 10;

        mHighKnobLayout.inLayout.x = 527;
        mHighKnobLayout.inLayout.y = 235;
        mHighKnobLayout.inLayout.frameWidth = 135;
        mHighKnobLayout.inLayout.frameHeight = 153;
        mHighKnobLayout.inLayout.ratio = 0.5f;
        mHighKnobLayout.inLayout.textboxHeight = 20;
        mHighKnobLayout.inLayout.textboxPadding = 10;

        mOutputKnobLayout.inLayout.x = 645;
        mOutputKnobLayout.inLayout.y = 235;
        mOutputKnobLayout.inLayout.frameWidth = 135;
        mOutputKnobLayout.inLayout.frameHeight = 153;
        mOutputKnobLayout.inLayout.ratio = 0.5f;
        mOutputKnobLayout.inLayout.textboxHeight = 20;
        mOutputKnobLayout.inLayout.textboxPadding = 10;

        mInputMeterLayout.inLayout.x = 100;
        mInputMeterLayout.inLayout.y = 225;
		mInputMeterLayout.inLayout.frameWidth = 70;
        mInputMeterLayout.inLayout.frameHeight = 60;
        mInputMeterLayout.inLayout.ratio = 0.5f;
		mInputMeterLayout.inLayout.textboxHeight = 20;
        mInputMeterLayout.inLayout.textboxPadding = 10;

        mOutputMeterLayout.inLayout.x = 750;
        mOutputMeterLayout.inLayout.y = 225;
		mOutputMeterLayout.inLayout.frameWidth = 70;
        mOutputMeterLayout.inLayout.frameHeight = 60;
		mOutputMeterLayout.inLayout.ratio = 0.5f;
        mOutputMeterLayout.inLayout.textboxHeight = 20;
		mOutputMeterLayout.inLayout.textboxPadding = 10;

        mIRButtonLayout.inLayout.x = 900;
        mIRButtonLayout.inLayout.y = 250;
        mIRButtonLayout.inLayout.frameWidth = 70;
        mIRButtonLayout.inLayout.frameHeight = 60;
		mIRButtonLayout.inLayout.ratio = 0.5f;
        mIRButtonLayout.inLayout.textboxHeight = 20;
		mIRButtonLayout.inLayout.textboxPadding = 10;

        mFileChooserButtonLayout.inLayout.x = 300;
        mFileChooserButtonLayout.inLayout.y = 500;
        mFileChooserButtonLayout.inLayout.frameWidth = 400;
		mFileChooserButtonLayout.inLayout.frameHeight = 40;
        mFileChooserButtonLayout.inLayout.ratio = 1.f;
		mFileChooserButtonLayout.inLayout.textboxHeight = 0;
        mFileChooserButtonLayout.inLayout.textboxPadding = 0;

        mNAMChooserButtonLayout.inLayout.x = 300;
        mNAMChooserButtonLayout.inLayout.y = 50;
        mNAMChooserButtonLayout.inLayout.frameWidth = 400;
        mNAMChooserButtonLayout.inLayout.frameHeight = 40;
        mNAMChooserButtonLayout.inLayout.ratio = 1.f;
        mNAMChooserButtonLayout.inLayout.textboxHeight = 0;
        mNAMChooserButtonLayout.inLayout.textboxPadding = 0;

        computeKnobLayout(mInputMeterLayout);
        computeKnobLayout(mInputKnobLayout);
        computeKnobLayout(mGateKnobLayout);
        computeKnobLayout(mBassKnobLayout);
        computeKnobLayout(mMidKnobLayout);
        computeKnobLayout(mHighKnobLayout);
        computeKnobLayout(mOutputKnobLayout);
        computeKnobLayout(mOutputMeterLayout);
        computeKnobLayout(mIRButtonLayout);
        computeKnobLayout(mFileChooserButtonLayout);
        computeKnobLayout(mNAMChooserButtonLayout);
    }

    void computeKnobLayout(KnobLayout& inKnobLayout)
    {
	    inKnobLayout.outLayout.x = inKnobLayout.inLayout.x;
        inKnobLayout.outLayout.y = inKnobLayout.inLayout.y;
        inKnobLayout.outLayout.sliderWidth = inKnobLayout.inLayout.frameWidth * inKnobLayout.inLayout.ratio;
        inKnobLayout.outLayout.sliderHeight = (inKnobLayout.inLayout.frameHeight + inKnobLayout.inLayout.textboxHeight + inKnobLayout.inLayout.textboxPadding) * inKnobLayout.inLayout.ratio;
	}

    void openFileChooser();
    void openNAMFileChooser();
    void handleSelectedFile(const juce::File& file);
    void handleSelectedNAMFile(const juce::File& file);

    void configureNodes(juce::AudioProcessor& gainProcessor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    class MeterComponent : public juce::Component, private juce::Timer
    {
    public:
        MeterComponent(juce::AudioProcessor& processor, bool isInput = true)
            : audioProcessor(processor)
            , isInput(isInput)
        {
            startTimerHz(30);
        }

        void paint(juce::Graphics& g) override;


    private:
        bool isInput;
        juce::AudioProcessor& audioProcessor;

        void timerCallback() override
        {
            repaint();
        }
    };
private:
    juce::Image mImage;
    MeterComponent mInputMeter;
    KnobLayout mInputMeterLayout;

    MeterComponent mOutputMeter;
	KnobLayout mOutputMeterLayout;


    juce::Slider mInputKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mInputAttachment;
    KnobLayout mInputKnobLayout;

    juce::Slider mGateKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGateKnobAttachment;
    KnobLayout mGateKnobLayout;

    juce::Slider mBassKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mBassKnobAttachment;
    KnobLayout mBassKnobLayout;

    juce::Slider mMidKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mMidKnobAttachment;
    KnobLayout mMidKnobLayout;

    juce::Slider mHighKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mHighKnobAttachment;
    KnobLayout mHighKnobLayout;

    juce::Slider mOutputKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mOutputKnobAttachment;
    KnobLayout mOutputKnobLayout;

    juce::ToggleButton mIRButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mIRButtonAttachment;
    KnobLayout mIRButtonLayout;

    juce::TextButton mFileChooserButton;
    KnobLayout mFileChooserButtonLayout;

    juce::TextButton mNAMChooserButton;
    KnobLayout mNAMChooserButtonLayout;

    juce::FileChooser mFileChooser;

    KnobLookAndFeel mKnobLookAndFeel; 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RootViewComponent)
};