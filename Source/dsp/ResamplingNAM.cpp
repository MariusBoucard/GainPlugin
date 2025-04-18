#include "ResamplingNAM.h"

ResamplingNAM::ResamplingNAM(std::unique_ptr<nam::DSP> encapsulated, const double expected_sample_rate)
: nam::DSP(expected_sample_rate)
, mEncapsulated(std::move(encapsulated))
{
    auto ProcessBlockFunc = [&](NAM_SAMPLE** input, NAM_SAMPLE** output, int numFrames) {
        mEncapsulated->process(input[0], output[0], numFrames);
        };
    mBlockProcessFunc = ProcessBlockFunc;

    if (mEncapsulated->HasLoudness())
    {
        SetLoudness(mEncapsulated->GetLoudness());
    }
    if (mEncapsulated->HasInputLevel())
    {
        SetInputLevel(mEncapsulated->GetInputLevel());
    }
    if (mEncapsulated->HasOutputLevel())
    {
        SetOutputLevel(mEncapsulated->GetOutputLevel());
    }

    int maxBlockSize = 2048; // Conservative
    Reset(expected_sample_rate, maxBlockSize);
};

double ResamplingNAM::GetNAMSampleRate(const std::unique_ptr<nam::DSP>& model)
{
    // Some models are from when we didn't have sample rate in the model.
    // For those, this wraps with the assumption that they're 48k models, which is probably true.
    const double assumedSampleRate = 48000.0;
    const double reportedEncapsulatedSampleRate = model->GetExpectedSampleRate();
    const double encapsulatedSampleRate =
        reportedEncapsulatedSampleRate <= 0.0 ? assumedSampleRate : reportedEncapsulatedSampleRate;
    return encapsulatedSampleRate;
};


void ResamplingNAM::process(NAM_SAMPLE* input, NAM_SAMPLE* output, const int num_frames)
{
    if (num_frames > mMaxExternalBlockSize)
        // We can afford to be careful
        throw std::runtime_error("More frames were provided than the max expected!");

    if (!NeedToResample())
    {
        mEncapsulated->process(input, output, num_frames);
    }
    else
    {
      //  mResampler.ProcessBlock(&input, &output, num_frames, mBlockProcessFunc);
    }
};

void ResamplingNAM::prewarm()
{
    mEncapsulated->prewarm();
};


void ResamplingNAM::Reset(const double sampleRate, const int maxBlockSize) 
{
    mExpectedSampleRate = sampleRate;
    mMaxExternalBlockSize = maxBlockSize;

    // Allocations in the encapsulated model (HACK)
    // Stolen some code from the resampler; it'd be nice to have these exposed as methods? :)
    const double mUpRatio = sampleRate / GetEncapsulatedSampleRate();
    const auto maxEncapsulatedBlockSize = static_cast<int>(std::ceil(static_cast<double>(maxBlockSize) / mUpRatio));
    mEncapsulated->ResetAndPrewarm(sampleRate, maxEncapsulatedBlockSize);
};


// So that we can let the world know if we're resampling (useful for debugging)
double ResamplingNAM::GetEncapsulatedSampleRate() const { return 44000; };

bool ResamplingNAM::NeedToResample() const
{
		// Check if the encapsulated model's sample rate is different from the expected sample rate
		return GetEncapsulatedSampleRate() != mExpectedSampleRate;
	};