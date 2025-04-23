class ParamListener : public juce::AudioProcessorValueTreeState::Listener
{
public:
    ParamListener(dsp::tone_stack::AbstractToneStack* toneStack, dsp::noise_gate::Gain* inGain, dsp::noise_gate::Trigger* inNoiseGateTrigger, ParameterSetup& inParameterSetup)
        : mToneStack(toneStack)
        , mNoiseGateGain(inGain)
        , mNoiseGateTrigger(inNoiseGateTrigger)
        , mParameterSetup(inParameterSetup)
    {}

    void parameterChanged(const juce::String& parameterID, float newValue) override
    {
        if (parameterID == "bass")
        {
            Mappers::setToneStackBass(mParameterSetup.mBassParams, newValue);
            mToneStack->SetParam("bass", newValue);
        }
        else if (parameterID == "mid")
        {
            Mappers::setToneStackMid(mParameterSetup.mMidParams, newValue);
            mToneStack->SetParam("middle", newValue);
        }
        else if (parameterID == "high")
        {
            Mappers::setToneStackHigh(mParameterSetup.mHighParams, newValue);
            mToneStack->SetParam("high", newValue);
        }
        else if (parameterID == "gate")
        {
            Mappers::setNoiseGateParams(mParameterSetup.mNoiseGateParams, newValue);
            mNoiseGateTrigger->SetParams(mParameterSetup.mNoiseGateParams);
            mNoiseGateTrigger->SetSampleRate(44100); // TODO
        }
    }
private:
    dsp::tone_stack::AbstractToneStack* mToneStack;
    dsp::noise_gate::Gain* mNoiseGateGain;
    dsp::noise_gate::Trigger* mNoiseGateTrigger;
    ParameterSetup& mParameterSetup;
};