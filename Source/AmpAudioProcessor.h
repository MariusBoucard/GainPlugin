
/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             GainPlugin
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Gain audio plugin.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_plugin_client, juce_audio_processors,
                   juce_audio_utils, juce_core, juce_data_structures,
                   juce_events, juce_graphics, juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2022

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             AudioProcessor
 mainClass:        GainProcessor

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once
#include "gui.h"
#include "NAM/dsp.h"
#include "dsp/ToneStack.h"
#include "dsp/NoiseGate.h"
#include "dsp/ImpulseResponse.h"
#include "NAM/get_dsp.h"

enum EParams
{
    // These need to be the first ones because I use their indices to place
    // their rects in the GUI.
    kInputLevel = 0,
    kNoiseGateThreshold,
    kToneBass,
    kToneMid,
    kToneTreble,
    kOutputLevel,
    // The rest is fine though.
    kNoiseGateActive,
    kEQActive,
    kIRToggle,
    // Input calibration
    kCalibrateInput,
    kInputCalibrationLevel,
    kOutputMode,
    kNumParams
};

const int numKnobs = 6;

//==============================================================================
class AmpAudioProcessor final : public AudioProcessor
{
public:

    //==============================================================================
    AmpAudioProcessor();

    ~AmpAudioProcessor() override;
    double getNAMSamplerate(const std::unique_ptr<nam::DSP>& inModel);



juce::AudioProcessorValueTreeState::ParameterLayout AmpAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat> ("input", "Input", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("gate", "Gate", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("bass", "Bass", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("mid", "Mid", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("high", "High", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("output", "Output", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("irEnabled", "IREnabled", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("namEnabled", "NAMEnabled", true));

    return { params.begin(), params.end() };
}
    //==============================================================================
    void prepareToPlay (double, int) override {}
    void releaseResources() override {}

    void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override;



    //==============================================================================
    AudioProcessorEditor* createEditor() override          { return new RootViewComponent (*this); }
    bool hasEditor() const override                        { return true;   }

    //==============================================================================
    const String getName() const override                  { return "Ballzzy's NAM"; }
    bool acceptsMidi() const override                      { return false; }
    bool producesMidi() const override                     { return false; }
    double getTailLengthSeconds() const override           { return 0; }

    //==============================================================================
    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return "None"; }
    void changeProgramName (int, const String&) override   {}

    void loadImpulseResponse(const juce::File& path);
    void loadNAMFile(const juce::File& path);
    dsp::wav::LoadReturnCode stageIR(const juce::File& path);
	
    class ParamListener : public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        ParamListener(dsp::tone_stack::AbstractToneStack* toneStack,dsp::noise_gate::Gain* inGain, dsp::noise_gate::Trigger* inNoiseGateTrigger)
            : mToneStack(toneStack)
            , mNoiseGateGain(inGain)
             ,mNoiseGateTrigger(inNoiseGateTrigger){}

        void parameterChanged(const juce::String& parameterID, float newValue) override
        {
            if (parameterID == "bass")
            {
                mToneStack->SetParam("bass", newValue);
            }
            else if (parameterID == "mid")
            {
                mToneStack->SetParam("middle", newValue);
            }
            else if (parameterID == "high")
            {
                mToneStack->SetParam("high", newValue);
            }
            else if (parameterID == "gate")
            {
                const double time = 0.01;
                const double threshold = newValue* (-160); // TODO Link param
                const double ratio = 0.1; // Quadratic...
                const double openTime = 0.005;
                const double holdTime = 0.01;
                const double closeTime = 0.05;
                const dsp::noise_gate::TriggerParams triggerParams(time, threshold, ratio, openTime, holdTime, closeTime);
                mNoiseGateTrigger->SetParams(triggerParams);
                mNoiseGateTrigger->SetSampleRate(44100);
            }
        }

    private:
         dsp::tone_stack::AbstractToneStack* mToneStack; // Pointer to the tone stack
         dsp::noise_gate::Gain* mNoiseGateGain; // Pointer to the noise gate gain
         dsp::noise_gate::Trigger* mNoiseGateTrigger; // Pointer to the noise gate trigger
	
    };
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
       // MemoryOutputStream (destData, true).writeFloat (*gain);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        //MemoryInputStream (data, sizeInBytes, false).readFloat (*mParameters);
      //  mParameters.setStateInformation (data, sizeInBytes);
    }

    double getRmsLevelLeft() const { return mRmsLevelLeft.load(); }
    double getRmsLevelRight() const { return mRmsLevelRight.load(); }
    double getRmsOutputLevelLeft() const { return mRmsOutputLevelLeft.load(); }
	double getRmsOutputLevelRight() const { return mRmsOutputLevelRight.load(); }

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        return true;
        const auto& mainInLayout  = layouts.getChannelSet (true,  0);
        const auto& mainOutLayout = layouts.getChannelSet (false, 0);

        return (mainInLayout == mainOutLayout && (! mainInLayout.isDisabled()));
    }

    juce::AudioProcessorValueTreeState& AmpAudioProcessor::getCustomParameterTree()
    {
        return mParameters;
    }

private:
    //==============================================================================
    AudioParameterFloat* gain;
    juce::AudioProcessorValueTreeState mParameters; 
    std::unique_ptr<nam::DSP> mModel;
    dsp::tone_stack::AbstractToneStack* mToneStack;
    dsp::noise_gate::Gain* mNoiseGateGain;
    dsp::noise_gate::Trigger* mNoiseGateTrigger;
    std::unique_ptr<dsp::ImpulseResponse> mIR;
    std::unique_ptr<dsp::ImpulseResponse> mStagedIR;
    juce::File mIRPath;
    bool mIsIRActive;
    bool mIsNAMEnabled;

    float** mFloatBuffer = nullptr; 
    float** mTempFloatBuffer = nullptr; 

    double** mDoubleBuffer = nullptr;
    double** mTempDoubleBuffer = nullptr;

    ParamListener mParamListener;

    private:
        std::atomic<float> mRmsLevelLeft{ 0.0f };
        std::atomic<float> mRmsLevelRight{ 0.0f };
        std::atomic<float> mRmsOutputLevelLeft{ 0.0f };
        std::atomic<float> mRmsOutputLevelRight{ 0.0f };

    double mBlockSize;
    double mSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpAudioProcessor)
};
