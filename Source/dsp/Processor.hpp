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
            const double time = 0.01;
            const double threshold = newValue * (-160); // TODO Link param
            const double ratio = 0.1; // Quadratic...
            const double openTime = 0.005;
            const double holdTime = 0.01;
            const double closeTime = 0.05;
            // TO MOVE TO MAPPERS
            const dsp::noise_gate::TriggerParams triggerParams(time, threshold, ratio, openTime, holdTime, closeTime);
            mNoiseGateTrigger->SetParams(triggerParams);
            mNoiseGateTrigger->SetSampleRate(44100);
        }
    }
private:
    dsp::tone_stack::AbstractToneStack* mToneStack;
    dsp::noise_gate::Gain* mNoiseGateGain;
    dsp::noise_gate::Trigger* mNoiseGateTrigger;
    ParameterSetup& mParameterSetup;
};