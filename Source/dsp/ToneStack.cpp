#include "ToneStack.h"

DSP_SAMPLE** dsp::tone_stack::BasicNamToneStack::Process(DSP_SAMPLE** inputs, const int numChannels,
                                                         const int numFrames)
{
  DSP_SAMPLE** bassPointers = mToneBass.Process(inputs, numChannels, numFrames);
  DSP_SAMPLE** midPointers = mToneMid.Process(bassPointers, numChannels, numFrames);
  DSP_SAMPLE** treblePointers = mToneTreble.Process(midPointers, numChannels, numFrames);
  return treblePointers;
}

void dsp::tone_stack::BasicNamToneStack::Reset(const double sampleRate, const int maxBlockSize)
{
  dsp::tone_stack::AbstractToneStack::Reset(sampleRate, maxBlockSize);

  // Refresh the params!
  SetParam("bass", mBassVal);
  SetParam("middle", mMiddleVal);
  SetParam("treble", mTrebleVal);
}

void dsp::tone_stack::BasicNamToneStack::SetParam(const std::string name, const double val)
{
  if (name == "bass")
  {
    mBassVal = val;
    const double sampleRate = GetSampleRate();
    const double bassGainDB = 10 * (val - 0.5); // +/- 10
    const double bassFrequency = 400.0;
    const double bassQuality = 0.707;
    recursive_linear_filter::BiquadParams bassParams(sampleRate, bassFrequency, bassQuality, bassGainDB);
    mToneBass.SetParams(bassParams);
  }
  else if (name == "middle")
  {
    mMiddleVal = val;
    const double sampleRate = GetSampleRate();
    const double midGainDB = 10 * (val - 0.5); // +/- 10
    const double midFrequency = 425.0;
    const double midQuality = midGainDB < 0.0 ? 1.5 : 0.7;
    recursive_linear_filter::BiquadParams midParams(sampleRate, midFrequency, midQuality, midGainDB);
    mToneMid.SetParams(midParams);
  }
  else if (name == "treble")
  {
    mTrebleVal = val;
    const double sampleRate = GetSampleRate();
    const double trebleGainDB  = 10 * (val - 0.5); // +/- 10
    const double trebleFrequency = 1800.0;
    const double trebleQuality = 0.707;
    recursive_linear_filter::BiquadParams trebleParams(sampleRate, trebleFrequency, trebleQuality, trebleGainDB);
    mToneTreble.SetParams(trebleParams);
  }
}
