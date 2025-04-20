#include "TopBarComponent.h"

void TopBarComponent::paint(juce::Graphics& g)
{

    g.setColour(juce::Colours::blue);
    juce::Rectangle<float> bounds(0, 0, getWidth(), getHeight());
    g.fillRect(bounds);


}