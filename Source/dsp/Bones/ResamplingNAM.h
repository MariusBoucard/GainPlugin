#pragma once

#include "NAM/dsp.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <cmath>

class ResamplingNAM : public nam::DSP
{
public:
    // Constructor
    ResamplingNAM(std::unique_ptr<nam::DSP> encapsulated, const double expected_sample_rate);

    // Override methods
    void prewarm() override;
    void process(NAM_SAMPLE* input, NAM_SAMPLE* output, const int num_frames) override;
    int GetLatency() const;
    void Reset(const double sampleRate, const int maxBlockSize) override;
    // Debugging utility
    double GetEncapsulatedSampleRate() const;
    double GetNAMSampleRate(const std::unique_ptr<nam::DSP>& model);

private:
    // Helper methods
    bool NeedToResample() const;

    // Member variables
    std::unique_ptr<nam::DSP> mEncapsulated;
    int mMaxExternalBlockSize = 0;
    std::function<void(NAM_SAMPLE**, NAM_SAMPLE**, int)> mBlockProcessFunc;
};