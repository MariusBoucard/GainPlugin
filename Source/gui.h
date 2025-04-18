#pragma once
#include "looknfeel/StripKnob_look_n_feel.h"
//#include "GainPluginDemo.h"
#include <JuceHeader.h>
class RootViewComponent : public juce::AudioProcessorEditor
{
public:
    static int ROOT_WIDTH;
    static int ROOT_HEIGHT;
    RootViewComponent(juce::AudioProcessor& processor); // Constructor takes a reference
    
    void configureNodes(juce::AudioProcessor& gainProcessor);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::Image mImage;
    juce::Slider mGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment; // Add this
    KnobLookAndFeel mKnobLookAndFeel; // Custom LookAndFeel for the knob
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RootViewComponent)
};