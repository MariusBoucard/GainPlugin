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
    , mBlockSize(256)
    , mSampleRate(44100)
    , mToneStack(std::make_unique<dsp::tone_stack::BasicNamToneStack>())
{
    setRateAndBufferSizeDetails(mSampleRate, mBlockSize);
    // Pre-allocate double** buffer if not already allocated
    if (!mDoubleBuffer)
    {
        mDoubleBuffer = new double* [getNumInputChannels()];
        for (int channel = 0; channel < getNumInputChannels(); ++channel)
        {
            mDoubleBuffer[channel] = new double[mBlockSize];
        }
    }
    if (!mTempDoubleBuffer)
    {
        mTempDoubleBuffer = new double* [getNumInputChannels()];
        for (int channel = 0; channel < getNumInputChannels(); ++channel)
        {
            mTempDoubleBuffer[channel] = new double[mBlockSize];
        }
    }
    mToneStack->Reset(getSampleRate(), getBlockSize());

    mModel = nam::get_dsp(std::filesystem::path("C:\\Users\\Marius\\Desktop\\JUCE\\projects\\GainPlugin\\Library\\NeuralAmpModelerCore\\example_models\\Metal lead.nam"));//loadModel("kk");
    this->createEditor();
    mModel->ResetAndPrewarm(mSampleRate, mBlockSize); // Set the sample rate and block size
}

void AmpAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
    buffer.applyGain(mParameters.getParameterAsValue("input").getValue()); // Apply input gain

    int isMono = 1;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();



    //// Copy float data from buffer to double**
    for (int channel = 0; channel < isMono; ++channel)
    {
        auto* floatData = buffer.getReadPointer(channel);
        std::copy(floatData, floatData + numSamples, mDoubleBuffer[channel]);
    }

    //// Process the model in chunks
    const int maxBlockSize = mBlockSize;
    for (int startSample = 0; startSample < numSamples; startSample += maxBlockSize)
    {
        const int blockSize = std::min(maxBlockSize, numSamples - startSample);

     
    for (int channel = 0; channel < isMono; ++channel)
        {
         //   mModel->process(mDoubleBuffer[channel] + startSample, mTempDoubleBuffer[channel] + startSample, blockSize);
        }
    }
    
    //// Copy processed data back to the buffer



    //// Set tone stack parameters
    mToneStack->SetParam("bass", mParameters.getParameterAsValue("bass").getValue());
    mToneStack->SetParam("middle", mParameters.getParameterAsValue("mid").getValue());
    mToneStack->SetParam("high", mParameters.getParameterAsValue("high").getValue());

    //// Process the tone stack
    mTempDoubleBuffer = mToneStack->Process(mDoubleBuffer, isMono, numSamples);

    ////// Copy processed data back to the buffer
    //for (int channel = 0; channel < numChannels; ++channel)
    //{
    //    auto* floatData = buffer.getWritePointer(channel);
    //    std::copy(mDoubleBuffer[channel], mDoubleBuffer[channel] + numSamples, floatData);
    //}

    for (int channel = 0; channel < isMono; ++channel)
    {
        auto* floatData = buffer.getWritePointer(channel);
        std::copy(mTempDoubleBuffer[channel], mTempDoubleBuffer[channel] + numSamples, floatData);
    }

    //// Handle mono-to-stereo processing
    //bool mMonoToStereo = true;
    //if (mMonoToStereo && numChannels > 1)
    //{
    //    auto* floatData = buffer.getWritePointer(1);
    //    auto* monoData = buffer.getWritePointer(0);
    //    std::copy(monoData, monoData + numSamples, floatData);
    //}
}