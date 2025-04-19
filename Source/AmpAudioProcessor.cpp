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

juce::File createJucePathFromFile(const juce::String& filePath)
{
    juce::File file(filePath); // Create a juce::File object from the file path
    if (!file.existsAsFile())
    {
        DBG("File does not exist: " << filePath);
        return juce::File(); // Return an empty file if it doesn't exist
    }


    return file;
}

AmpAudioProcessor::AmpAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::mono())
    . withOutput("Output", AudioChannelSet::stereo()))
    , mParameters(*this, nullptr, "PARAMETERS", createParameterLayout()) // Initialize parameters
    , mBlockSize(256)
    , mSampleRate(44100)
    , mToneStack(new dsp::tone_stack::BasicNamToneStack())
    , mNoiseGateTrigger(new dsp::noise_gate::Trigger())
	, mNoiseGateGain(new dsp::noise_gate::Gain())
    , mParamListener(mToneStack, mNoiseGateGain, mNoiseGateTrigger)
    , mIsNAMEnabled(true)
    , mIsIRActive(false)
    , mIRPath(createJucePathFromFile("D:\\Projets musique\\vst\\Amps\\Revv V30 Fredman Impulse Response\\Wav\\Revv.wav"))
{
    mNoiseGateTrigger->AddListener(mNoiseGateGain);

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
    mParameters.addParameterListener("gate", &mParamListener);
    
    mToneStack->Reset(getSampleRate(), getBlockSize());

    mModel = nam::get_dsp(std::filesystem::path("C:\\Users\\Marius\\Desktop\\JUCE\\projects\\GainPlugin\\Library\\NeuralAmpModelerCore\\example_models\\Metal lead.nam"));
    mModel->ResetAndPrewarm(mSampleRate, mBlockSize);

    loadImpulseResponse(mIRPath);

    this->createEditor();
}
AmpAudioProcessor::~AmpAudioProcessor()
{
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

    for (int channel = 0; channel < isMono; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);
        float sum = 0.0f;

        for (int i = 0; i < numSamples; ++i)
        {
            sum += channelData[i] * channelData[i];
        }

        float rms = std::sqrt(sum / numSamples);

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

    mTempDoubleBuffer = mNoiseGateTrigger->Process(mDoubleBuffer, isMono, numSamples);

    if (mIsNAMEnabled and mParameters.getParameterAsValue("namEnabled").getValue())
    {
        const int maxBlockSize = mBlockSize;
        for (int startSample = 0; startSample < numSamples; startSample += maxBlockSize)
        {
            const int blockSize = std::min(maxBlockSize, numSamples - startSample);


            for (int channel = 0; channel < isMono; ++channel)
            {

                mModel->process(mTempDoubleBuffer[channel] + startSample, mDoubleBuffer[channel] + startSample, blockSize);
            }
        }
    }
    else
	{
		for (int channel = 0; channel < isMono; ++channel)
		{
			std::copy(mTempDoubleBuffer[channel], mTempDoubleBuffer[channel] + numSamples, mDoubleBuffer[channel]);
		}
	}
    
    mTempDoubleBuffer = mNoiseGateGain->Process(mDoubleBuffer, isMono, numSamples);


    mDoubleBuffer = mToneStack->Process(mTempDoubleBuffer, isMono, numSamples);

    if (mIR != nullptr and mParameters.getParameterAsValue("irEnabled").getValue()) // TODO add param to trigger path
        mTempDoubleBuffer = mIR->Process(mDoubleBuffer, isMono, numSamples);
    else
    {
			for (int channel = 0; channel < isMono; ++channel)
			{
				std::copy(mDoubleBuffer[channel], mDoubleBuffer[channel] + numSamples, mTempDoubleBuffer[channel]);
			}
    }


    for (int channel = 0; channel < isMono; ++channel)
    {
        std::transform(mTempDoubleBuffer[channel], mTempDoubleBuffer[channel] + numSamples, mTempFloatBuffer[channel], [](double sample) {
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
void AmpAudioProcessor::loadImpulseResponse(const juce::File& inIRFile)
{
    stageIR(inIRFile);
    if (mStagedIR != nullptr)
	{
		mIR = std::move(mStagedIR);
		mStagedIR = nullptr;
	}
	else
	{
		std::cerr << "Failed to load impulse response." << std::endl;
	}
}

void AmpAudioProcessor::loadNAMFile(const juce::File& inNAMFile)
{
	try
	{
        mIsNAMEnabled = false;
		mModel = nam::get_dsp(std::filesystem::path(inNAMFile.getFullPathName().toStdString()));
		mModel->ResetAndPrewarm(mSampleRate, mBlockSize); // Set the sample rate and block size
        mIsNAMEnabled = true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error loading NAM file: " << e.what() << std::endl;
	}
}

dsp::wav::LoadReturnCode AmpAudioProcessor::stageIR(const juce::File& irPath)
{
    juce::File previousIRPath = mIRPath;
    const double sampleRate = getSampleRate();
    dsp::wav::LoadReturnCode wavState = dsp::wav::LoadReturnCode::ERROR_OTHER;
    try
    {
        juce::String irPathString = irPath.getFullPathName();;
        const char* irPathChar = irPathString.toRawUTF8();    // Convert to const char*

        mStagedIR = std::make_unique<dsp::ImpulseResponse>(irPathChar, sampleRate);
        wavState = mStagedIR->GetWavState();
    }
    catch (std::runtime_error& e)
    {
        wavState = dsp::wav::LoadReturnCode::ERROR_OTHER;
        std::cerr << "Caught unhandled exception while attempting to load IR:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    if (wavState == dsp::wav::LoadReturnCode::SUCCESS)
    {
        mIRPath = irPath;
     
    }
    else
    {
        if (mStagedIR != nullptr)
        {
            mStagedIR = nullptr;
        }
        mIRPath = previousIRPath;
    }

    return wavState;
}