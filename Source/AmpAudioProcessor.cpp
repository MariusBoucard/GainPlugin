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
    , mToneStack(new dsp::tone_stack::BasicNamToneStack())
    , mParamListener(mToneStack)
{
    setRateAndBufferSizeDetails(mSampleRate, mBlockSize);
    // Pre-allocate double** buffer if not already allocated
    if (!mFloatBuffer)
    {
        mFloatBuffer = new float* [getNumInputChannels()];
        for (int channel = 0; channel < getNumInputChannels(); ++channel)
        {
            mFloatBuffer[channel] = new float[1024];
        }
    }
    if (!mTempFloatBuffer)
    {
        mTempFloatBuffer = new float* [getNumInputChannels()];
        for (int channel = 0; channel < getNumInputChannels(); ++channel)
        {
            mTempFloatBuffer[channel] = new float[1024];
        }
    }
    mParameters.addParameterListener("bass", &mParamListener);
    mParameters.addParameterListener("mid", &mParamListener);
    mParameters.addParameterListener("high", &mParamListener);

    mToneStack->Reset(getSampleRate(), getBlockSize());
    mModel = nam::get_dsp(std::filesystem::path("C:\\Users\\Marius\\Desktop\\JUCE\\projects\\GainPlugin\\Library\\NeuralAmpModelerCore\\example_models\\Metal lead.nam"));//loadModel("kk");
    this->createEditor();
    mModel->ResetAndPrewarm(mSampleRate, mBlockSize); // Set the sample rate and block size
}
AmpAudioProcessor::~AmpAudioProcessor()
{
	// Clean up double** buffer
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
		delete[] mFloatBuffer[channel];
		delete[] mTempFloatBuffer[channel];
	}
	delete[] mFloatBuffer;
	delete[] mTempFloatBuffer;
}

void AmpAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
   buffer.applyGain(mParameters.getParameterAsValue("input").getValue()); // Apply input gain
   
    int isMono = 1;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // Calculate RMS levels
    for (int channel = 0; channel < isMono; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);
        float sum = 0.0f;

        for (int i = 0; i < numSamples; ++i)
        {
            sum += channelData[i] * channelData[i];
        }

        float rms = std::sqrt(sum / numSamples);

        // Update the atomic variables
        if (channel == 0)
        {
            mRmsLevelLeft.store(rms);
            mRmsLevelRight.store(rms);
        }
    }

    if (!mFloatBuffer)
    {
		mFloatBuffer = new float* [isMono];
		mTempFloatBuffer = new float* [isMono];
        for (int channel = 0; channel < isMono; ++channel)
        {
			mFloatBuffer[channel] = new float[1024];
			mTempFloatBuffer[channel] = new float[1024];
		}
	}

    if (!mDoubleBuffer)
    {
        mDoubleBuffer = new double* [isMono];
        mTempDoubleBuffer = new double* [isMono];
        for (int channel = 0; channel < isMono; ++channel)
        {
			mDoubleBuffer[channel] = new double[1024];
            mTempDoubleBuffer[channel] = new double[1024];
		}
    }
    for (int channel = 0; channel < isMono; ++channel)
    {
        auto* floatData = buffer.getReadPointer(channel);
        std::copy(floatData, floatData + numSamples, mFloatBuffer[channel]);
    }


    for (int channel = 0; channel < isMono; ++channel)
    {
        std::transform(mFloatBuffer[channel], mFloatBuffer[channel] + numSamples, mDoubleBuffer[channel], [](float sample) {
            return static_cast<double>(sample);
            });
    }

    const int maxBlockSize = mBlockSize;
    for (int startSample = 0; startSample < numSamples; startSample += maxBlockSize)
    {
        const int blockSize = std::min(maxBlockSize, numSamples - startSample);

    
    for (int channel = 0; channel < isMono; ++channel)
        {
    
          mModel->process(mDoubleBuffer[channel] + startSample, mTempDoubleBuffer[channel] + startSample, blockSize);
        }
    }
    



    mDoubleBuffer = mToneStack->Process(mTempDoubleBuffer, isMono, numSamples);

    for (int channel = 0; channel < isMono; ++channel)
    {
        std::transform(mDoubleBuffer[channel], mDoubleBuffer[channel] + numSamples, mTempFloatBuffer[channel], [](double sample) {
			return static_cast<float>(sample);
		});
	}
       for (int channel = 0; channel < isMono; ++channel)
    {
        auto* floatData = buffer.getWritePointer(channel);
        std::copy(mTempFloatBuffer[channel], mTempFloatBuffer[channel] + numSamples, floatData);
    }

    bool mMonoToStereo = true;
    if (mMonoToStereo && numChannels > 1)
    {
        auto* floatData = buffer.getWritePointer(1);
        auto* monoData = buffer.getReadPointer(0);
        std::copy(monoData, monoData + numSamples, floatData);
    }

    buffer.applyGain(mParameters.getParameterAsValue("output").getValue()); 

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);
        float sum = 0.0f;

        for (int i = 0; i < numSamples; ++i)
        {
            sum += channelData[i] * channelData[i];
        }

        float rms = std::sqrt(sum / numSamples);

        // Update the atomic variables
        if (channel == 0)
        {
            mRmsOutputLevelLeft.store(rms);
        }
        if (channel == 1)
        {
            mRmsOutputLevelRight.store(rms);
        }
    }
}