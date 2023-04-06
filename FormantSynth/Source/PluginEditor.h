/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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

private:
    FormantSynthAudioProcessor& audioProcessor;

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override;
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

    void enableSourceGui(int sourceWave);

    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    /*GUI Objects*/
    // Colouring
    juce::Colour sliderTrackFore = juce::Colour(0xFFD0D0D0);

    juce::LookAndFeel_V4 textLookAndFeel;
    juce::LookAndFeel_V4 windowLookAndFeel;
    juce::LookAndFeel_V4 sourceLookAndFeel;
    juce::LookAndFeel_V4 fricativeLookAndFeel;
    juce::LookAndFeel_V4 filterLookAndFeel;
    juce::LookAndFeel_V4 vibratoLookAndFeel;
    juce::LookAndFeel_V4 mixerLookAndFeel;
    // Mixer
    juce::Slider fofGainSlider;
    juce::Label fofGainLabel;
    juce::Slider bpGainSlider;
    juce::Label bpGainLabel;
    juce::Slider fricativeGainSlider;
    juce::Label fricativeGainLabel; 

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

    // Filter

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessorEditor)
};
