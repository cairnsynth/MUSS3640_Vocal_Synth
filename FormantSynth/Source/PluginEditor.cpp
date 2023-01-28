/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FormantSynthAudioProcessorEditor::FormantSynthAudioProcessorEditor (FormantSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

    addAndMakeVisible(vowelSlider);
    vowelSlider.setRange(0, 4);
    vowelSlider.setValue(0);
    vowelSlider.setNumDecimalPlacesToDisplay(0);
    vowelSlider.onValueChange = [this] {
        audioProcessor.setFormantFreqs(vowelSlider.getValue());
        audioProcessor.setFormantGains(vowelSlider.getValue());
        audioProcessor.setFormantBandwidths(vowelSlider.getValue());
    };

    addAndMakeVisible(vowelLabel);
    vowelLabel.setText("Vowel", juce::dontSendNotification);
    vowelLabel.attachToComponent(&vowelSlider, true);

    addAndMakeVisible(freqSlider);
    freqSlider.setRange(20.0, 20000.0);
    freqSlider.setNumDecimalPlacesToDisplay(0);
    freqSlider.setValue(220.0);
    freqSlider.onValueChange = [this] {
        audioProcessor.setInFreq(freqSlider.getValue());
    };
}

FormantSynthAudioProcessorEditor::~FormantSynthAudioProcessorEditor()
{
}

//==============================================================================
void FormantSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void FormantSynthAudioProcessorEditor::resized()
{
    const int sliderLeft = 80;
    vowelSlider.setBounds(sliderLeft, 100, getWidth() - sliderLeft - 20, 20);
    freqSlider.setBounds(sliderLeft, 150, getWidth() - sliderLeft - 20, 20);
}
