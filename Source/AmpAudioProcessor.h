
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

//==============================================================================
class AmpAudioProcessor final : public AudioProcessor
{
public:

    //==============================================================================
    AmpAudioProcessor()
        : AudioProcessor (BusesProperties().withInput  ("Input",  AudioChannelSet::stereo())
                                           .withOutput ("Output", AudioChannelSet::stereo()))
         , mParameters(*this, nullptr, "PARAMETERS", createParameterLayout()) // Initialize parameters

    {
        this->createEditor();
        addParameter (gain = new AudioParameterFloat ({ "gain", 1 }, "Gain", 0.0f, 1.0f, 0.5f));
    }


    
juce::AudioProcessorValueTreeState::ParameterLayout AmpAudioProcessor::createParameterLayout()
{
    // Define the gain parameter
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("input", "Input", 0.0f, 1.0f, 0.5f));
    return { params.begin(), params.end() };
}
    //==============================================================================
    void prepareToPlay (double, int) override {}
    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        buffer.applyGain (*gain);
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer&) override
    {
        buffer.applyGain ((float) *gain);
    }

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

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
        MemoryOutputStream (destData, true).writeFloat (*gain);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        gain->setValueNotifyingHost (MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat());
    }

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
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

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpAudioProcessor)
};
