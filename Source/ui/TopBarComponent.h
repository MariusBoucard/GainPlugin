#include <JuceHeader.h>

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
        mSettingButton.onClick = [this]() { showSettingsModal(); };

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

private:
    juce::AudioProcessor& audioProcessor;
    juce::TextButton mAboutButton{ "About" };
    juce::TextButton mSettingButton{ "settings" };
    juce::Label mAmpNameLabel{ "Amp Name" };

    void showAboutModal()
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::InfoIcon,
            "About",
            "Ballzzy's DSP | NAM - version 0.1",
            "Lets get back to it");
    }
    void showSettingsModal()
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::InfoIcon,
            "About",
            "Ballzzy's DSP | NAM - version 0.1",
            "Lets get back to it");
    }

};