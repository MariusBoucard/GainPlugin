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

AmpAudioProcessor::AmpAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::mono())
    . withOutput("Output", AudioChannelSet::stereo()))
    , mParameters(*this, nullptr, "PARAMETERS", createParameterLayout()) // Initialize parameters
    , mBlockSize(512)
    , mSampleRate(44100)
{
    mModel = nam::get_dsp(std::filesystem::path("C:\\Users\\Marius\\Desktop\\JUCE\\projects\\GainPlugin\\Library\\NeuralAmpModelerCore\\example_models\\Metal lead.nam"));//loadModel("kk");
    this->createEditor();
    mModel->ResetAndPrewarm(mSampleRate, mBlockSize); // Set the sample rate and block size
}
void AmpAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
    buffer.applyGain(mParameters.getParameterAsValue("input").getValue()); // Use the parameter value directly

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    std::vector<std::vector<double>> doubleBuffer(numChannels, std::vector<double>(numSamples));

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* floatData = buffer.getReadPointer(channel);
        auto& doubleData = doubleBuffer[channel];
        std::copy(floatData, floatData + numSamples, doubleData.begin());
    }

    const int maxBlockSize = mBlockSize;
    for (int startSample = 0; startSample < numSamples; startSample += maxBlockSize)
    {
        const int blockSize = std::min(maxBlockSize, numSamples - startSample);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto& doubleData = doubleBuffer[channel];
            mModel->process(doubleData.data() + startSample, doubleData.data() + startSample, blockSize);
        }
    }

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* floatData = buffer.getWritePointer(channel);
        auto& doubleData = doubleBuffer[channel];
        std::transform(doubleData.begin(), doubleData.end(), floatData, [](double sample)
            {
                return static_cast<float>(sample);
            });
    }

    bool mMonoToStereo = true;
    if (mMonoToStereo)
    {
       auto* floatData = buffer.getWritePointer(1);
       auto& doubleData = doubleBuffer[0];
       std::transform(doubleData.begin(), doubleData.end(), floatData, [](double sample) {
                return static_cast<float>(sample);
           });
    }
 }