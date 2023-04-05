/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FormantSynthAudioProcessorEditor::FormantSynthAudioProcessorEditor (FormantSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize (800, 500);
    setResizable(true, false);

    /*GUI Initialisation*/

    //Keyboard
    addAndMakeVisible(keyboardComponent);
    keyboardState.addListener(this);
    //Bandpass source selection box
    addAndMakeVisible(bpSourceWaveBox);
    bpSourceWaveBox.addItemList({ "Square", "Sawtooth", "Glottal Model" }, 1);
    bpSourceWaveBox.setSelectedItemIndex(0);
    bpSourceWaveBox.onChange = [this] {
        audioProcessor.setBpSourceWave(bpSourceWaveBox.getSelectedId());
        enableSourceGui(bpSourceWaveBox.getSelectedId());
    };
    //Bandpass source pulsewidth (square)
    addAndMakeVisible(bpSourcePwSlider);
    bpSourcePwSlider.setRange(0.0, 1.0);
    bpSourcePwSlider.setValue(0.5);
    bpSourcePwSlider.onValueChange = [this] {
        audioProcessor.setBpSourcePwValue(bpSourcePwSlider.getValue());
    };
    addAndMakeVisible(bpSourcePwLabel);
    bpSourcePwLabel.setText("Pulsewidth", juce::dontSendNotification);
    bpSourcePwLabel.attachToComponent(&bpSourcePwSlider, true);

    addAndMakeVisible(fofGainSlider);
    fofGainSlider.setRange(0.0, 1.0);
    fofGainSlider.setValue(1.0);
    fofGainSlider.onValueChange = [this] {
        audioProcessor.setFofGain(fofGainSlider.getValue());
    };
    addAndMakeVisible(fofGainLabel);
    fofGainLabel.setText("FOF Gain", juce::dontSendNotification);
    fofGainLabel.attachToComponent(&fofGainSlider, true);

    addAndMakeVisible(bpGainSlider);
    bpGainSlider.setRange(0.0, 1.0);
    bpGainSlider.setValue(1.0);
    bpGainSlider.onValueChange = [this] {
        audioProcessor.setBpGain(bpGainSlider.getValue());
    };
    addAndMakeVisible(bpGainLabel);
    bpGainLabel.setText("BP Gain", juce::dontSendNotification);
    bpGainLabel.attachToComponent(&bpGainSlider, true);

    addAndMakeVisible(fricativeGainSlider);
    fricativeGainSlider.setRange(0.0, 1.0);
    fricativeGainSlider.setValue(1.0);
    fricativeGainSlider.onValueChange = [this] {
        audioProcessor.setBpGain(fricativeGainSlider.getValue());
    };
    addAndMakeVisible(fricativeGainLabel);
    fricativeGainLabel.setText("Fricative Gain", juce::dontSendNotification);
    fricativeGainLabel.attachToComponent(&fricativeGainSlider, true);
}

FormantSynthAudioProcessorEditor::~FormantSynthAudioProcessorEditor()
{
    keyboardState.removeListener(this);
}

//==============================================================================
void FormantSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::dimgrey);
}

void FormantSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto headerHeight = 30;
    auto keyboardHeight = 60;
    const int sliderLeft = 80;

    bpSourceWaveBox.setBounds(10, 10, 200, 20);
    bpSourcePwSlider.setBounds(30, sliderLeft, area.getWidth() - sliderLeft - 20, 20);

    keyboardComponent.setBounds(area.removeFromBottom(keyboardHeight));
    fofGainSlider.setBounds(sliderLeft, 120, getWidth() - sliderLeft - 20, 20);
    bpGainSlider.setBounds(sliderLeft, 160, getWidth() - sliderLeft - 20, 20);
    fricativeGainSlider.setBounds(sliderLeft, 200, getWidth() - sliderLeft - 20, 20);

}

void FormantSynthAudioProcessorEditor::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
}

void FormantSynthAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    audioProcessor.keyOn(midiNoteNumber, int(127 * velocity));
}

void FormantSynthAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    audioProcessor.keyOff(midiNoteNumber);
}

void FormantSynthAudioProcessorEditor::enableSourceGui(int sourceWave)
{
    switch (sourceWave) {
    default :
    {
        bpSourcePwSlider.setEnabled(true);
        break;
    }
    case 2:
    {
        bpSourcePwSlider.setEnabled(false);
        break;
    }
    case 3:
    {
        bpSourcePwSlider.setEnabled(false);
        break;
    }
    }
}