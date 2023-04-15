#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Phoneme.h"

//==============================================================================
/**
*/
class FormantSynthAudioProcessorEditor  : 
    public juce::AudioProcessorEditor,
    private juce::MidiInputCallback,
    private juce::MidiKeyboardStateListener
{
public:
    FormantSynthAudioProcessorEditor (FormantSynthAudioProcessor&);
    ~FormantSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void updateFilterControls(Phoneme p);
    void updateFormant(int f, float freq, float bw, float gain);

    //juce::MidiKeyboardState keyboardState;

private:
    FormantSynthAudioProcessor& audioProcessor;

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override;
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

    void enableSourceGui(int sourceWave);

    

    
    juce::MidiKeyboardComponent keyboardComponent;

    /*GUI Objects*/
    // Colouring
    juce::Colour sliderTrackFore = juce::Colour(0xFFC0C0C0);
    juce::Colour sliderTrackBack = juce::Colour(0xFFB0B0B0);

    juce::LookAndFeel_V4 textLookAndFeel;
    juce::LookAndFeel_V4 windowLookAndFeel;
    juce::LookAndFeel_V4 sourceLookAndFeel;
    juce::LookAndFeel_V4 fricativeLookAndFeel;
    juce::LookAndFeel_V4 filterLookAndFeel;
    juce::LookAndFeel_V4 vibratoLookAndFeel;
    juce::LookAndFeel_V4 mixerLookAndFeel;
    juce::LookAndFeel_V4 disabledLookAndFeel;
    // Mixer
    juce::Slider fofGainSlider;
    juce::Label fofGainLabel;
    juce::ToggleButton fofGainLockButton;
    juce::Slider bpGainSlider;
    juce::Label bpGainLabel;
    juce::ToggleButton bpGainLockButton;
    juce::Slider fricativeGainSlider;
    juce::Label fricativeGainLabel; 
    juce::ToggleButton fricativeGainLockButton;

    // Source
    juce::Label bpSourceLabel;
    juce::ComboBox bpSourceWaveBox;
    juce::Slider bpSourcePwSlider;
    juce::Label bpSourcePwLabel;
    juce::Slider bpSourcePressureSlider;
    juce::Label bpSourcePressureLabel;
    juce::Slider bpSourceT0Slider;
    juce::Label bpSourceT0Label;
    juce::Slider bpSourceTeSlider;
    juce::Label bpSourceTeLabel;
    juce::Slider bpSourceNoiseSlider;
    juce::Label bpSourceNoiseLabel;
    juce::Label fricativeSourceLabel;
    juce::Slider fricativeColourSlider;
    juce::Slider fricativeLowCutSlider;
    juce::Slider fricativeHighCutSlider;
    juce::ToggleButton monoButton;
    

    // Filter
    juce::Label phonemeLabel;
    juce::Slider phonemeSlider;
    juce::Label formantShiftLabel;
    juce::Slider formantShiftSlider;

    juce::TextButton addPhonemeButton;

    juce::TextButton loadXmlButton;
    std::unique_ptr<juce::XmlElement> fileData;
    juce::XmlElement* columnList = nullptr;
    juce::XmlElement* dataList = nullptr;

    juce::Slider f1FreqSlider;
    juce::Slider f1BandwidthSlider;
    juce::Slider f1GainSlider;

    juce::Slider f2FreqSlider;
    juce::Slider f2BandwidthSlider;
    juce::Slider f2GainSlider;

    juce::Slider f3FreqSlider;
    juce::Slider f3BandwidthSlider;
    juce::Slider f3GainSlider;

    juce::Slider f4FreqSlider;
    juce::Slider f4BandwidthSlider;
    juce::Slider f4GainSlider;

    juce::Slider f5FreqSlider;
    juce::Slider f5BandwidthSlider;
    juce::Slider f5GainSlider;

    // Envelopes
    juce::Label voiceEnvelopeLabel;
    juce::Slider voiceAttackSlider;
    juce::Label voiceAttackLabel;
    juce::Slider voiceDecaySlider;
    juce::Label voiceDecayLabel;
    juce::Slider voiceSustainSlider;
    juce::Label voiceSustainLabel;
    juce::Slider voiceReleaseSlider;
    juce::Label voiceReleaseLabel;

    juce::Label fricativeEnvelopeLabel;
    juce::Slider fricativeAttackSlider;
    juce::Label fricativeAttackLabel;
    juce::Slider fricativeDecaySlider;
    juce::Label fricativeDecayLabel;
    juce::Slider fricativeSustainSlider;
    juce::Label fricativeSustainLabel;
    juce::Slider fricativeReleaseSlider;
    juce::Label fricativeReleaseLabel;

    // Vibrato
    juce::Label vibratoLabel;
    juce::Slider vibratoFrequencySlider;
    juce::Label vibratoFrequencyLabel;
    juce::Slider vibratoAttackSlider;
    juce::Label vibratoAttackLabel;
    juce::Slider vibratoSustainSlider;
    juce::Label vibratoSustainLabel;
    juce::Slider vibratoReleaseSlider;
    juce::Label vibratoReleaseLabel;

    // Graphics
    juce::TextButton headerWindow;
    juce::TextButton sourceWindow;
    juce::TextButton filterWindow;
    juce::TextButton vibratoWindow;
    juce::TextButton envelopeWindow;
    juce::TextButton mixerWindow;

    // GUI element attachments must be destructed before the associated gui element
    // Destructed from bottom up
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> bpSourceWaveComboAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bpSourcePwSliderAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bpSourcePressureAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bpSourceT0Attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bpSourceTeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bpSourceNoiseAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monoAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fricativeLowCutAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fricativeHighCutAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phonemeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> formantShiftAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f1FreqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f1BandwidthAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f1GainAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f2FreqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f2BandwidthAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f2GainAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f3FreqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f3BandwidthAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f3GainAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f4FreqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f4BandwidthAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f4GainAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f5FreqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f5BandwidthAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> f5GainAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoFrequencyAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoAttackAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoSustainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoReleaseAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceAttachAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceDecayAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceSustainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceReleaseAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fricativeAttackAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fricativeDecayAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fricativeSustainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fricativeReleaseAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fofGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> fofGainLockAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bpGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bpGainLockAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fricativeGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> fricativeGainLockAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessorEditor)
};
