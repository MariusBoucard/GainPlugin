#include "TopBarComponent.h"
#include "../AmpAudioProcessor.h"
void TopBarComponent::paint(juce::Graphics& g)
{

    g.setColour(juce::Colours::blue);
    juce::Rectangle<float> bounds(0, 0, getWidth(), getHeight());
    g.fillRect(bounds);


}

void TopBarComponent::showSettingsModal(AudioProcessor& inProcessor)
{
    auto* modal = new SettingsModal(inProcessor);

    if (auto* parentEditor = dynamic_cast<juce::AudioProcessorEditor*>(getParentComponent()))
    {
        parentEditor->addAndMakeVisible(modal);
        modal->setBounds(parentEditor->getLocalBounds()); 
    }
    modal->setAlwaysOnTop(true);
    modal->setVisible(true);
    modal->enterModalState(false, nullptr, true);

}

void TopBarComponent::SettingsModal::openDirectoryChooser()
{
    juce::FileChooser chooser("Select a Directory",
                            juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                            "*",
                            true);


        if (chooser.browseForDirectory())
        {
            handleSelectedDirectory(chooser.getResult());
        }
    
}

void TopBarComponent::SettingsModal::handleSelectedDirectory(const juce::File& directory)
{
    // Example: Save the selected directory path to the processor
        if (directory.exists() && directory.isDirectory())
	        {
		        // Assuming you have a method in your processor to handle the directory
                AmpAudioProcessor * processor = dynamic_cast<AmpAudioProcessor*>(&mProcessor);
            if (processor == nullptr)
            {
	            DBG("Failed to cast inProcessor to AmpAudioProcessor.");
	            return;
            }
            else
            {
                processor->setNAMPath(directory.getFullPathName());
                if (auto* parentEditor = dynamic_cast<RootViewComponent*>(getParentComponent()))
                {
                    parentEditor->updatePath(mProcessor);
                }
            }
	    }
	    else
	    {
		    DBG("Selected path is not a valid directory.");
	    }
    DBG("Handling directory: " << directory.getFullPathName());

    // You can pass this directory to your processor or store it in a parameter
}