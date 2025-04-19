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
{
    mModel = nam::get_dsp(std::filesystem::path("C:\\Users\\Marius\\Desktop\\JUCE\\projects\\GainPlugin\\Library\\NeuralAmpModelerCore\\example_models\\Metal lead.nam"));//loadModel("kk");
    this->createEditor();
    mModel->ResetAndPrewarm(44100, 512); // Set the sample rate and block size
    addParameter(gain = new AudioParameterFloat({ "gain", 1 }, "Gain", 0.0f, 1.0f, 0.5f));
}
void AmpAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
    // Apply gain before processing
    buffer.applyGain(mParameters.getParameterAsValue("input").getValue()); // Use the parameter value directly
   // buffer.applyGain(*gain);

    // Get the number of channels and samples
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // Create a temporary buffer for double-precision data
    std::vector<std::vector<double>> doubleBuffer(numChannels, std::vector<double>(numSamples));

    // Convert the float buffer to double
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* floatData = buffer.getReadPointer(channel);
        auto& doubleData = doubleBuffer[channel];
        std::copy(floatData, floatData + numSamples, doubleData.begin());
    }

    // Process the buffer in chunks to avoid exceeding the model's max block size
    const int maxBlockSize = 512; // Replace with the actual max block size of mModel
    for (int startSample = 0; startSample < numSamples; startSample += maxBlockSize)
    {
        const int blockSize = std::min(maxBlockSize, numSamples - startSample);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto& doubleData = doubleBuffer[channel];
            mModel->process(doubleData.data() + startSample, doubleData.data() + startSample, blockSize);
        }
    }

    // Convert the processed double data back to float
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* floatData = buffer.getWritePointer(channel);
        auto& doubleData = doubleBuffer[channel];
        std::transform(doubleData.begin(), doubleData.end(), floatData, [](double sample) {
            return static_cast<float>(sample);
            });
    }

    bool mMonoToStereo = false;
    if (mMonoToStereo)
    {
       auto* floatData = buffer.getWritePointer(1);
       auto& doubleData = doubleBuffer[0];
       std::transform(doubleData.begin(), doubleData.end(), floatData, [](double sample) {
           return static_cast<float>(sample);
           });
    }

 }