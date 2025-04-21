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

void TopBarComponent::SettingsModal::openDirectoryChooser(DirectoryType inDirectoryType)
{
    juce::FileChooser chooser("Select a Directory",
                            juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                            "*",
                            true);


        if (chooser.browseForDirectory())
        {
            handleSelectedDirectory(chooser.getResult(), inDirectoryType);
        }
    
}

void TopBarComponent::SettingsModal::handleSelectedDirectory(const juce::File& directory, DirectoryType inDirectoryType)
{
        if (directory.exists() && directory.isDirectory())
	        {
                AmpAudioProcessor * processor = dynamic_cast<AmpAudioProcessor*>(&mProcessor);
            if (processor == nullptr)
            {
	            DBG("Failed to cast inProcessor to AmpAudioProcessor.");
	            return;
            }
            else
            {
                switch (inDirectoryType)
                {
                case TopBarComponent::SettingsModal::DirectoryType::NAMDir:
                    processor->setNAMPath(directory.getFullPathName());
                    break;
                case TopBarComponent::SettingsModal::DirectoryType::IRDir:
                    processor->setIRPath(directory.getFullPathName());
                    break;
                case TopBarComponent::SettingsModal::DirectoryType::VerbIRDir:
                    break;
                default:
                    break;
                }
                if (auto* parentEditor = dynamic_cast<RootViewComponent*>(getParentComponent()))
                {
                    parentEditor->updatePath();
                }
            }
	    }
	    else
	    {
		    DBG("Selected path is not a valid directory.");
	    }
    DBG("Handling directory: " << directory.getFullPathName());

}