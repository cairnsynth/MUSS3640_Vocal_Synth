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

    //GUI Objects

    juce::Slider fofGainSlider;
    juce::Label fofGainLabel;
    juce::Slider bpGainSlider;
    juce::Label bpGainLabel;
    juce::Slider fricativeGainSlider;
    juce::Label fricativeGainLabel;

    juce::ComboBox bpSourceWaveBox;
    juce::Slider bpSourcePwSlider;
    juce::Label bpSourcePwLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessorEditor)
};
