#pragma once
#include "../looknfeel/StripKnob_look_n_feel.h"
#include "../looknfeel/ToogleButtonLookAndFeel.h"
#include "../looknfeel/TopBarLookAndFeel.h"
#include "TopBarComponent.h"
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
    ~RootViewComponent() override;

    void defineKnobLayout()
    {
        mTopBarLayout.inLayout.x = 0;
        mTopBarLayout.inLayout.y = 0;
        mTopBarLayout.inLayout.frameWidth = getLocalBounds().getWidth();
        mTopBarLayout.inLayout.frameHeight = 30;
        mTopBarLayout.inLayout.ratio = 1.f;
        mTopBarLayout.inLayout.textboxHeight = 0;
        mTopBarLayout.inLayout.textboxPadding = 0;

        mInputKnobLayout.inLayout.x = 180;
		mInputKnobLayout.inLayout.y = 235;
		mInputKnobLayout.inLayout.frameWidth = 135;
		mInputKnobLayout.inLayout.frameHeight = 153;
		mInputKnobLayout.inLayout.ratio = 0.5f;
		mInputKnobLayout.inLayout.textboxHeight = 20;
        mInputKnobLayout.inLayout.textboxPadding = -15;

        mGateKnobLayout.inLayout.x = 260;
		mGateKnobLayout.inLayout.y = 235;
        mGateKnobLayout.inLayout.frameWidth = 135;
		mGateKnobLayout.inLayout.frameHeight = 153;
        mGateKnobLayout.inLayout.ratio = 0.5f;
		mGateKnobLayout.inLayout.textboxHeight = 20;
        mGateKnobLayout.inLayout.textboxPadding = -15;

        mBassKnobLayout.inLayout.x = 390;
        mBassKnobLayout.inLayout.y = 235;
        mBassKnobLayout.inLayout.frameWidth = 135;
        mBassKnobLayout.inLayout.frameHeight = 153;
        mBassKnobLayout.inLayout.ratio = 0.5f;
        mBassKnobLayout.inLayout.textboxHeight = 20;
        mBassKnobLayout.inLayout.textboxPadding = -15;

        mMidKnobLayout.inLayout.x = 460;
        mMidKnobLayout.inLayout.y = 235;
        mMidKnobLayout.inLayout.frameWidth = 135;
        mMidKnobLayout.inLayout.frameHeight = 153;
        mMidKnobLayout.inLayout.ratio = 0.5f;
        mMidKnobLayout.inLayout.textboxHeight = 20;
        mMidKnobLayout.inLayout.textboxPadding = -15;

        mHighKnobLayout.inLayout.x = 527;
        mHighKnobLayout.inLayout.y = 235;
        mHighKnobLayout.inLayout.frameWidth = 135;
        mHighKnobLayout.inLayout.frameHeight = 153;
        mHighKnobLayout.inLayout.ratio = 0.5f;
        mHighKnobLayout.inLayout.textboxHeight = 20;
        mHighKnobLayout.inLayout.textboxPadding = -15;

        mOutputKnobLayout.inLayout.x = 645;
        mOutputKnobLayout.inLayout.y = 235;
        mOutputKnobLayout.inLayout.frameWidth = 135;
        mOutputKnobLayout.inLayout.frameHeight = 153;
        mOutputKnobLayout.inLayout.ratio = 0.5f;
        mOutputKnobLayout.inLayout.textboxHeight = 20;
        mOutputKnobLayout.inLayout.textboxPadding = -15;

        mInputMeterLayout.inLayout.x = 125;
        mInputMeterLayout.inLayout.y = 232;
		mInputMeterLayout.inLayout.frameWidth = 70;
        mInputMeterLayout.inLayout.frameHeight = 60;
        mInputMeterLayout.inLayout.ratio = 0.5f;
		mInputMeterLayout.inLayout.textboxHeight = 20;
        mInputMeterLayout.inLayout.textboxPadding = 10;

        mOutputMeterLayout.inLayout.x = 725;
        mOutputMeterLayout.inLayout.y = 232;
		mOutputMeterLayout.inLayout.frameWidth = 85;
        mOutputMeterLayout.inLayout.frameHeight = 60;
		mOutputMeterLayout.inLayout.ratio = 0.5f;
        mOutputMeterLayout.inLayout.textboxHeight = 20;
		mOutputMeterLayout.inLayout.textboxPadding = 10;

        mIRButtonLayout.inLayout.x = 680;
        mIRButtonLayout.inLayout.y = 455;
        mIRButtonLayout.inLayout.frameWidth = 30;
        mIRButtonLayout.inLayout.frameHeight = 30;
		mIRButtonLayout.inLayout.ratio = 1.f;
        mIRButtonLayout.inLayout.textboxHeight = 0;
		mIRButtonLayout.inLayout.textboxPadding = 0;

        mNAMButtonLayout.inLayout.x = 660;
        mNAMButtonLayout.inLayout.y = 60;
        mNAMButtonLayout.inLayout.frameWidth = 30;
        mNAMButtonLayout.inLayout.frameHeight = 30;
        mNAMButtonLayout.inLayout.ratio = 1.f;
        mNAMButtonLayout.inLayout.textboxHeight = 0;
        mNAMButtonLayout.inLayout.textboxPadding = 0;

        mFileChooserButtonLayout.inLayout.x = 320;
        mFileChooserButtonLayout.inLayout.y = 452;
        mFileChooserButtonLayout.inLayout.frameWidth = 350;
		mFileChooserButtonLayout.inLayout.frameHeight = 18;
        mFileChooserButtonLayout.inLayout.ratio = 1.f;
		mFileChooserButtonLayout.inLayout.textboxHeight = 0;
        mFileChooserButtonLayout.inLayout.textboxPadding = 0;

        mNAMChooserButtonLayout.inLayout.x = 327;
        mNAMChooserButtonLayout.inLayout.y = 65;
        mNAMChooserButtonLayout.inLayout.frameWidth = 323;
        mNAMChooserButtonLayout.inLayout.frameHeight = 12;
        mNAMChooserButtonLayout.inLayout.ratio = 1.f;
        mNAMChooserButtonLayout.inLayout.textboxHeight = 0;
        mNAMChooserButtonLayout.inLayout.textboxPadding = 0;

        mIRVerbButtonLayout.inLayout.x = 800;
        mIRVerbButtonLayout.inLayout.y = 200;
		mIRVerbButtonLayout.inLayout.frameWidth = 30;
        mIRVerbButtonLayout.inLayout.frameHeight = 30;
		mIRVerbButtonLayout.inLayout.ratio = 1.f;
        mIRVerbButtonLayout.inLayout.textboxHeight = 0;
		mIRVerbButtonLayout.inLayout.textboxPadding = 0;

        mVerbMixKnobLayout.inLayout.x = 780;
		mVerbMixKnobLayout.inLayout.y = 235;
        mVerbMixKnobLayout.inLayout.frameWidth = 135;
		mVerbMixKnobLayout.inLayout.frameHeight = 153;
        mVerbMixKnobLayout.inLayout.ratio = 0.5f;
		mVerbMixKnobLayout.inLayout.textboxHeight = 20;
        mVerbMixKnobLayout.inLayout.textboxPadding = -15;

        mVerbDropDownLayout.inLayout.x = 735;
        mVerbDropDownLayout.inLayout.y = 185;
        mVerbDropDownLayout.inLayout.frameWidth = 110;
        mVerbDropDownLayout.inLayout.frameHeight = 5;
        mVerbDropDownLayout.inLayout.ratio = 1.f;
        mVerbDropDownLayout.inLayout.textboxHeight = 0;
        mVerbDropDownLayout.inLayout.textboxPadding = 0;

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
        computeKnobLayout(mNAMButtonLayout);
        computeKnobLayout(mIRVerbButtonLayout);
        computeKnobLayout(mVerbMixKnobLayout);
        computeKnobLayout(mTopBarLayout);
        computeKnobLayout(mVerbDropDownLayout);
    }

    void computeKnobLayout(KnobLayout& inKnobLayout)
    {
	    inKnobLayout.outLayout.x = inKnobLayout.inLayout.x;
        inKnobLayout.outLayout.y = inKnobLayout.inLayout.y;
        inKnobLayout.outLayout.sliderWidth = inKnobLayout.inLayout.frameWidth * inKnobLayout.inLayout.ratio;
        inKnobLayout.outLayout.sliderHeight = (inKnobLayout.inLayout.frameHeight + inKnobLayout.inLayout.textboxHeight + 10) * inKnobLayout.inLayout.ratio;
	}

    void handleSelectedFile(const juce::File& file);
    void handleSelectedNAMFile(const juce::File& file);
    void handleSelectedVerbIRFile(const juce::File& file);

    void updatePath();
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

    juce::Slider mVerbMixKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mVerbMixKnobAttachment;
    KnobLayout mVerbMixKnobLayout;

    juce::ToggleButton mIRButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mIRButtonAttachment;
    KnobLayout mIRButtonLayout;

    juce::ToggleButton mIRVerbButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mIRVerbButtonAttachment;
    KnobLayout mIRVerbButtonLayout;

    juce::ToggleButton mNAMButton;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mNAMButtonAttachment;
    KnobLayout mNAMButtonLayout;

    juce::ComboBox mFileChooserButton;
    KnobLayout mFileChooserButtonLayout;

    KnobLayout mNAMChooserButtonLayout;
    juce::ComboBox mNAMChooserButton;

    juce::ComboBox mVerbDropDown;
    KnobLayout mVerbDropDownLayout;

    juce::OwnedArray<juce::File> mIRFileList;
    juce::OwnedArray<juce::File> mNAMFileList;
    juce::OwnedArray<juce::File> mVerbFileList;


    TopBarComponent mTopBar;

    KnobLayout mTopBarLayout;


    juce::FileChooser mFileChooser;

    KnobLookAndFeel mKnobLookAndFeel; 
    DropDownLookAndFeel mDropDownLookAndFeel; 
    ToggleButtonLookAndFeel mToggleLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RootViewComponent)
};