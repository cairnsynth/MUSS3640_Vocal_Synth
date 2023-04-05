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


    addAndMakeVisible(&guiGraphic);
    guiGraphic.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    guiGraphic.setButtonText("GUI");

    // Keyboard
    addAndMakeVisible(&keyboardComponent);
    keyboardState.addListener(this);
    // Bandpass source selection box
    addAndMakeVisible(&bpSourceWaveBox);
    bpSourceWaveBox.addItemList({ "Square", "Sawtooth", "Glottal Model" }, 1);
    bpSourceWaveBox.setSelectedItemIndex(0);
    bpSourceWaveBox.onChange = [this] {
        audioProcessor.setBpSourceWave(bpSourceWaveBox.getSelectedId());
        enableSourceGui(bpSourceWaveBox.getSelectedId());
    };
    // Bandpass source pulsewidth (square)
    addAndMakeVisible(&bpSourcePwSlider);
    bpSourcePwSlider.setRange(0.0, 1.0);
    bpSourcePwSlider.setValue(0.5);
    bpSourcePwSlider.onValueChange = [this] {
        audioProcessor.setBpSourcePw(bpSourcePwSlider.getValue());
    };
    addAndMakeVisible(&bpSourcePwLabel);
    bpSourcePwLabel.setText("Pulsewidth", juce::dontSendNotification);
    bpSourcePwLabel.attachToComponent(&bpSourcePwSlider, false);
    // Bandpass source pressure (glottal model)
    addAndMakeVisible(&bpSourcePressureSlider);
    bpSourcePressureSlider.setRange(0.0, 1.0);
    bpSourcePressureSlider.setValue(1.0);
    bpSourcePressureSlider.onValueChange = [this] {
        audioProcessor.setBpSourcePressure(bpSourcePressureSlider.getValue());
    };
    addAndMakeVisible(&bpSourcePressureLabel);
    bpSourcePressureLabel.setText("Pressure", juce::dontSendNotification);
    bpSourcePressureLabel.attachToComponent(&bpSourcePressureSlider, false);
    // Bandpass source T0 (glottal model)
    addAndMakeVisible(&bpSourceT0Slider);
    bpSourceT0Slider.setRange(0.0, 1.0);
    bpSourceT0Slider.setValue(1.0);
    bpSourceT0Slider.onValueChange = [this] {
        audioProcessor.setBpSourceT0(bpSourceT0Slider.getValue());
    };
    addAndMakeVisible(&bpSourceT0Label);
    bpSourceT0Label.setText("T0", juce::dontSendNotification);
    bpSourceT0Label.attachToComponent(&bpSourceT0Slider, false);
    // Bandpass source Te (glottal model)
    addAndMakeVisible(&bpSourceTeSlider);
    bpSourceTeSlider.setRange(0.0, 1.0);
    bpSourceTeSlider.setValue(1.0);
    bpSourceTeSlider.onValueChange = [this] {
        audioProcessor.setBpSourceTe(bpSourceTeSlider.getValue());
    };
    addAndMakeVisible(&bpSourceTeLabel);
    bpSourceTeLabel.setText("Te", juce::dontSendNotification);
    bpSourceTeLabel.attachToComponent(&bpSourceTeSlider, false);
    // Bandpass source noise (glottal model)
    addAndMakeVisible(&bpSourceNoiseSlider);
    bpSourceNoiseSlider.setRange(0.0, 1.0);
    bpSourceNoiseSlider.setValue(1.0);
    bpSourceNoiseSlider.onValueChange = [this] {
        audioProcessor.setBpSourceNoise(bpSourceNoiseSlider.getValue());
    };
    addAndMakeVisible(&bpSourceNoiseLabel);
    bpSourceNoiseLabel.setText("Noise", juce::dontSendNotification);
    bpSourceNoiseLabel.attachToComponent(&bpSourceNoiseSlider, false);
    // FOF Gain
    addAndMakeVisible(&fofGainSlider);
    fofGainSlider.setRange(0.0, 1.0);
    fofGainSlider.setValue(1.0);
    fofGainSlider.onValueChange = [this] {
        audioProcessor.setFofGain(fofGainSlider.getValue());
    };
    addAndMakeVisible(&fofGainLabel);
    fofGainLabel.setText("FOF Gain", juce::dontSendNotification);
    fofGainLabel.attachToComponent(&fofGainSlider, false);
    // BP Gain
    addAndMakeVisible(&bpGainSlider);
    bpGainSlider.setRange(0.0, 1.0);
    bpGainSlider.setValue(1.0);
    bpGainSlider.onValueChange = [this] {
        audioProcessor.setBpGain(bpGainSlider.getValue());
    };
    addAndMakeVisible(&bpGainLabel);
    bpGainLabel.setText("BP Gain", juce::dontSendNotification);
    bpGainLabel.attachToComponent(&bpGainSlider, false);
    // Fricative Gain
    addAndMakeVisible(&fricativeGainSlider);
    fricativeGainSlider.setRange(0.0, 1.0);
    fricativeGainSlider.setValue(1.0);
    fricativeGainSlider.onValueChange = [this] {
        audioProcessor.setFricativeGain(fricativeGainSlider.getValue());
    };
    addAndMakeVisible(&fricativeGainLabel);
    fricativeGainLabel.setText("Fricative Gain", juce::dontSendNotification);
    fricativeGainLabel.attachToComponent(&fricativeGainSlider, false);

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

    //g.setColour(juce::Colours::white);
    //g.fillRect(0, headerBottom, getWidth(), guiBorderWidth);
    //g.fillRect(sourceAreaRight, headerBottom, guiBorderWidth, getHeight() - keyboardTop);
    //g.fillRect(filterAreaRight, headerBottom, guiBorderWidth, getHeight() - keyboardTop);
}

void FormantSynthAudioProcessorEditor::resized()
{
    // GUI Area variables
    const int objectBorder = 5;
    const int objectHeight = 40;
    const int keyboardHeight = 60;
    const int headerHeight = 30;
    auto area = getLocalBounds();
    auto headerArea = area.removeFromTop(headerHeight);
    auto keyboardArea = area.removeFromBottom(keyboardHeight);
    auto sourceArea = area.removeFromLeft(juce::jmax(100, getWidth() / 4));
    auto filterArea = area.removeFromLeft(juce::jmax(200, area.getWidth() / 2));
    auto saveArea = area.removeFromBottom(area.getHeight() / 5);
    auto envelopeArea = area.removeFromBottom(area.getHeight() / 2);
    auto vibratoArea = area;

    const int sliderLeft = 80;

    keyboardComponent.setBounds(keyboardArea);
    guiGraphic.setBounds(sourceArea);

    bpSourceWaveBox.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourcePwSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourcePressureSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourceT0Slider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourceTeSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourceNoiseSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));

    fofGainSlider.setBounds(envelopeArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpGainSlider.setBounds(envelopeArea.removeFromTop(objectHeight).reduced(objectBorder));
    fricativeGainSlider.setBounds(envelopeArea.removeFromTop(objectHeight).reduced(objectBorder));

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
        bpSourcePressureSlider.setEnabled(false);
        bpSourceT0Slider.setEnabled(false);
        bpSourceTeSlider.setEnabled(false);
        bpSourceNoiseSlider.setEnabled(false);

        break;
    }
    case 3:
    {
        bpSourcePwSlider.setEnabled(false);
        bpSourcePressureSlider.setEnabled(true);
        bpSourceT0Slider.setEnabled(true);
        bpSourceTeSlider.setEnabled(true);
        bpSourceNoiseSlider.setEnabled(true);
        break;
    }
    }
}