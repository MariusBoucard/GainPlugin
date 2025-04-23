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

  SetParam("bass", mBassVal);
  SetParam("middle", mMiddleVal);
  SetParam("high", mTrebleVal);
}

void dsp::tone_stack::BasicNamToneStack::SetParam(const std::string name, const double val)
{
  if (name == "bass")
  {
    mBassVal = val;
    mToneBass.SetParams(mParameterSetup.mBassParams);
  }
  else if (name == "middle")
  {
    mMiddleVal = val;
    mToneMid.SetParams(mParameterSetup.mMidParams);
  }
  else if (name == "high")
  {
      mTrebleVal = val;
      mToneTreble.SetParams(mParameterSetup.mHighParams);
  }
}
