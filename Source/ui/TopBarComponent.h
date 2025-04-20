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

        addAndMakeVisible(mAmpNameLabel);
        mAmpNameLabel.setText("My Amp", juce::dontSendNotification); 
        mAmpNameLabel.setJustificationType(juce::Justification::centred); 
        mAmpNameLabel.setFont(juce::Font(18.0f, juce::Font::bold));
        mAmpNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        mAmpNameLabel.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
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
        SettingsModal(AudioProcessor& inProcessor)
            : mProcessor(inProcessor)
        {
            setLookAndFeel(&mLookAndFeel);

            addAndMakeVisible(mDirectoryChooserButton);
            mDirectoryChooserButton.setButtonText("Choose Directory");
            mDirectoryChooserButton.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
            mDirectoryChooserButton.onClick = [this]() { openDirectoryChooser(); };

            addAndMakeVisible(mCloseButton);
            mCloseButton.setButtonText("Close");
            mCloseButton.onClick = [this]() { exitModalState(0); };
        }
        ~SettingsModal() override {
            setLookAndFeel(nullptr); // Reset LookAndFeel to default
        };

        void paint(juce::Graphics& g) override
        {
            mLookAndFeel.drawComponentBackground(g, *this); // Use custom LookAndFeel to draw the background
        }

        void resized() override
        {
            auto area = getLocalBounds().reduced(40); // Add padding
            mTitleLabel.setBounds(area.removeFromTop(50)); // Title at the top
            mDirectoryChooserButton.setBounds(area.removeFromTop(50).reduced(0, 10));
            mCloseButton.setBounds(area.removeFromTop(50).reduced(0, 10));
        }

        void openDirectoryChooser();
        void handleSelectedDirectory(const juce::File& directory);
    private:
        juce::AudioProcessor& mProcessor;
        juce::TextButton mDirectoryChooserButton{ "Choose Directory" };
        juce::TextButton mCloseButton{ "Close" };
        juce::Label mTitleLabel; // Title label
        SettingsModalLookAndFeel mLookAndFeel; // Custom LookAndFeel
    };

private:
    void showSettingsModal(AudioProcessor& inProcessor);


private:
    juce::AudioProcessor& audioProcessor;
    juce::TextButton mAboutButton{ "About" };
    juce::TextButton mSettingButton{ "settings" };
    juce::TextButton mDirectoryChooserButton{ "Open Directory" };
    juce::Label mAmpNameLabel{ "Amp Name" };

    void showAboutModal()
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::InfoIcon,
            "About",
            "Ballzzy's DSP | NAM - version 0.1",
            "Lets get back to it");
    }


};