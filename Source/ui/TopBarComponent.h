#pragma once
#include <JuceHeader.h>
#include "../looknfeel/TopBarLookAndFeel.h"

class TopBarComponent : public juce::Component
{
public:
    TopBarComponent(juce::AudioProcessor& processor)
        : audioProcessor(processor)
    {

        addAndMakeVisible(mAboutButton);
        mAboutButton.setButtonText("L");
        mAboutButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        mAboutButton.onClick = [this]() { showAboutModal(); };

        addAndMakeVisible(mSettingButton);
        mSettingButton.setButtonText("R");
        mSettingButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        mSettingButton.onClick = [this]() { showSettingsModal(audioProcessor); };

		mAmpNameLabel.setLookAndFeel(&mLabelLookAndFeel);
        addAndMakeVisible(mAmpNameLabel);
        mAmpNameLabel.setText("Ballzzy Audio", juce::dontSendNotification); 
        mAmpNameLabel.setJustificationType(juce::Justification::centred); 
        mAmpNameLabel.setFont(juce::Font(18.0f, juce::Font::bold));

    }

    void paint(juce::Graphics& g) override;

    void resized() override
    {
        auto buttonSize = getHeight();
        mAboutButton.setBounds(0, 0, buttonSize, buttonSize);
        mSettingButton.setBounds(getWidth() - buttonSize, 0, buttonSize, buttonSize);
        mAmpNameLabel.setBounds(buttonSize, 0, getWidth() - 2 * buttonSize, buttonSize);
    }

    class SettingsModal : public juce::Component
    {
    public:

        enum class DirectoryType{
            NAMDir,
            IRDir,
            VerbIRDir
        } ;

        SettingsModal(AudioProcessor& inProcessor);
     
        ~SettingsModal() override {
mDirectoryChooserNAMButton.setLookAndFeel(nullptr);
		mDirectoryChooserIRButton.setLookAndFeel(nullptr);
mCloseButton.setLookAndFeel(nullptr);
		mTitleLabel.setLookAndFeel(nullptr);
mNAMPathLabel.setLookAndFeel(nullptr);
		mIRPathLabel.setLookAndFeel(nullptr);

            setLookAndFeel(nullptr); 
        };

        void paint(juce::Graphics& g) override
        {
            mLookAndFeel.drawComponentBackground(g, *this);
        }

        void resized() override
        {
            auto area = getLocalBounds().reduced(40); 
            mTitleLabel.setBounds(area.removeFromTop(50)); 
            mNAMPathLabel.setBounds(area.removeFromTop(30).reduced(0, 5));
            mDirectoryChooserNAMButton.setBounds(area.removeFromTop(50).reduced(0, 10));

            mIRPathLabel.setBounds(area.removeFromTop(30).reduced(0, 5));
            mDirectoryChooserIRButton.setBounds(area.removeFromTop(50).reduced(0, 10));

            mCloseButton.setBounds(area.removeFromTop(50).reduced(0, 10));
        }

        void openDirectoryChooser(DirectoryType inDirectoryType);
        void handleSelectedDirectory(const juce::File& directory, DirectoryType inDirectoryType);

    private:
        juce::AudioProcessor& mProcessor;
        juce::TextButton mDirectoryChooserNAMButton{ "Choose Directory" };
        juce::TextButton mDirectoryChooserIRButton{ "Close" };
        juce::TextButton mCloseButton{ "Close" };
        juce::Label mNAMPathLabel{ "No NAM Path Selected" };
        juce::Label mIRPathLabel{ "No IR Path Selected" };
        juce::Label mTitleLabel{"Amplifier Settings"};
        SettingsModalLookAndFeel mLookAndFeel; 
        ModalButtonDefaultLookAndFeel mModalButtonLookAndFeel;
    };

private:
    void showSettingsModal(AudioProcessor& inProcessor);


private:
    juce::AudioProcessor& audioProcessor;
    juce::TextButton mAboutButton{ "About" };
    juce::TextButton mSettingButton{ "settings" };
    juce::Label mAmpNameLabel{ "Amp Name" };
    BeautifulLabelLookAndFeel mLabelLookAndFeel; 

    void showAboutModal()
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::InfoIcon,
            "About",
            "Ballzzy's DSP | NAM - version 0.1",
            "Lets get back to it");
    }


};