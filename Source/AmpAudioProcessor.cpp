/*
  ==============================================================================

    AmpAudioProcessor.cpp
    Created: 18 Apr 2025 6:14:04pm
    Author:  Marius

  ==============================================================================
*/

#include "AmpAudioProcessor.h"

#include <filesystem>
#include <iostream>


juce::File AmpAudioProcessor::createJucePathFromFile(const juce::String& filePath)
{
    juce::File file(filePath); 
    if (!file.existsAsFile() and !file.exists())
    {
        DBG("File does not exist: " << filePath);
        return juce::File(); 
    }


    return file;
}

AmpAudioProcessor::AmpAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::mono())
    . withOutput("Output", AudioChannelSet::stereo()))
    , mParameters(*this, nullptr, "PARAMETERS", createParameterLayout())
    , mSkeletonAmpProcessor(mParameters)
{

}
AmpAudioProcessor::~AmpAudioProcessor()
{
   
}

void AmpAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
  mSkeletonAmpProcessor.processBlock(buffer, MidiBuffer());

}
