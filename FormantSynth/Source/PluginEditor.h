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
class FormantSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FormantSynthAudioProcessorEditor (FormantSynthAudioProcessor&);
    ~FormantSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider vowelSlider;
    juce::Label vowelLabel;
    juce::Slider freqSlider;

    FormantSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessorEditor)
};
