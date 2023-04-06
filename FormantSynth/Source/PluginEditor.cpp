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
    // Bandpass source label
    addAndMakeVisible(&bpSourceLabel);
    bpSourceLabel.setText("Bandpass Source", juce::dontSendNotification);
    // Fricative source label
    addAndMakeVisible(&fricativeSourceLabel);
    fricativeSourceLabel.setText("Fricative Source", juce::dontSendNotification);
    // Bandpass source selection box
    addAndMakeVisible(&bpSourceWaveBox);
    bpSourceWaveBox.addItemList({ "Square", "Sawtooth", "Glottal Model" }, 1);
    bpSourceWaveBox.onChange = [this] {
        audioProcessor.setBpSourceWave(bpSourceWaveBox.getSelectedId());
        enableSourceGui(bpSourceWaveBox.getSelectedId());
    };
    bpSourceWaveBox.setSelectedItemIndex(0);
    // Bandpass source pulsewidth (square)
    addAndMakeVisible(&bpSourcePwSlider);
    bpSourcePwSlider.setRange(0.0, 1.0);
    bpSourcePwSlider.setValue(0.5);
    bpSourcePwSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
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
    bpSourcePressureSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourcePressureSlider.onValueChange = [this] {
        audioProcessor.setBpSourcePressure(bpSourcePressureSlider.getValue());
    };
    addAndMakeVisible(&bpSourcePressureLabel);
    bpSourcePressureLabel.setText("Pressure", juce::dontSendNotification);
    bpSourcePressureLabel.attachToComponent(&bpSourcePressureSlider, false);
    // Bandpass source T0 (glottal model)
    addAndMakeVisible(&bpSourceT0Slider);
    bpSourceT0Slider.setRange(0.0, 1.0);
    bpSourceT0Slider.setValue(0.944);
    bpSourceT0Slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceT0Slider.onValueChange = [this] {
        audioProcessor.setBpSourceT0(bpSourceT0Slider.getValue());
    };
    addAndMakeVisible(&bpSourceT0Label);
    bpSourceT0Label.setText("T0", juce::dontSendNotification);
    bpSourceT0Label.attachToComponent(&bpSourceT0Slider, false);
    // Bandpass source Te (glottal model)
    addAndMakeVisible(&bpSourceTeSlider);
    bpSourceTeSlider.setRange(0.0, 1.0);
    bpSourceTeSlider.setValue(0.224);
    bpSourceTeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceTeSlider.onValueChange = [this] {
        audioProcessor.setBpSourceTe(bpSourceTeSlider.getValue());
    };
    addAndMakeVisible(&bpSourceTeLabel);
    bpSourceTeLabel.setText("Te", juce::dontSendNotification);
    bpSourceTeLabel.attachToComponent(&bpSourceTeSlider, false);
    // Bandpass source noise (glottal model)
    addAndMakeVisible(&bpSourceNoiseSlider);
    bpSourceNoiseSlider.setRange(0.0, 1.0);
    bpSourceNoiseSlider.setValue(0.0);
    bpSourceNoiseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceNoiseSlider.onValueChange = [this] {
        audioProcessor.setBpSourceNoise(bpSourceNoiseSlider.getValue());
    };
    addAndMakeVisible(&bpSourceNoiseLabel);
    bpSourceNoiseLabel.setText("Noise", juce::dontSendNotification);
    bpSourceNoiseLabel.attachToComponent(&bpSourceNoiseSlider, false);
    // Fricative colour slider
    addAndMakeVisible(&fricativeColourSlider);
    fricativeColourSlider.setSliderStyle(juce::Slider::TwoValueHorizontal);
    fricativeColourSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeColourSlider.setRange(20.0, 20000.0);
    fricativeColourSlider.setMinAndMaxValues(20.0, 20000.0, juce::dontSendNotification);
    fricativeColourSlider.onValueChange = [this] {
        audioProcessor.setFricativeColour(fricativeColourSlider.getMinValue(), fricativeColourSlider.getMaxValue());
    };

    // Voice envelope label
    addAndMakeVisible(&voiceEnvelopeLabel);
    voiceEnvelopeLabel.setText("Voice", juce::dontSendNotification);
    // Voice attack
    addAndMakeVisible(&voiceAttackSlider);
    voiceAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceAttackSlider.setRange(0.0, 2.0);
    voiceAttackSlider.setValue(0.05);
    voiceAttackSlider.onValueChange = [this] {
        audioProcessor.setVoiceAttack(voiceAttackSlider.getValue());
    };
    addAndMakeVisible(&voiceAttackLabel);
    voiceAttackLabel.setText("A", juce::dontSendNotification);
    voiceAttackLabel.attachToComponent(&voiceAttackSlider, false);
    // Voice decay
    addAndMakeVisible(&voiceDecaySlider);
    voiceDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceDecaySlider.setRange(0.0, 2.0);
    voiceDecaySlider.setValue(0.05);
    voiceDecaySlider.onValueChange = [this] {
        audioProcessor.setVoiceDecay(voiceDecaySlider.getValue());
    };
    addAndMakeVisible(&voiceDecayLabel);
    voiceDecayLabel.setText("D", juce::dontSendNotification);
    voiceDecayLabel.attachToComponent(&voiceDecaySlider, false);
    // Voice sustain
    addAndMakeVisible(&voiceSustainSlider);
    voiceSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceSustainSlider.setRange(0.0, 1.0);
    voiceSustainSlider.setValue(1.0);
    voiceSustainSlider.onValueChange = [this] {
        audioProcessor.setVoiceSustain(voiceSustainSlider.getValue());
    };
    addAndMakeVisible(&voiceSustainLabel);
    voiceSustainLabel.setText("S", juce::dontSendNotification);
    voiceSustainLabel.attachToComponent(&voiceSustainSlider, false);
    // Voice release
    addAndMakeVisible(&voiceReleaseSlider);
    voiceReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceReleaseSlider.setRange(0.0, 4.0);
    voiceReleaseSlider.setValue(1.0);
    voiceReleaseSlider.onValueChange = [this] {
        audioProcessor.setVoiceRelease(voiceReleaseSlider.getValue());
    };
    addAndMakeVisible(&voiceReleaseLabel);
    voiceReleaseLabel.setText("R", juce::dontSendNotification);
    voiceReleaseLabel.attachToComponent(&voiceReleaseSlider, false);
    // Fricative envelope label
    addAndMakeVisible(&fricativeEnvelopeLabel);
    fricativeEnvelopeLabel.setText("Fricative", juce::dontSendNotification);
    // Fricative attack

    // Fricative decay

    // Fricative sustain

    // Fricative release

    // FOF Gain
    addAndMakeVisible(&fofGainSlider);
    fofGainSlider.setRange(0.0, 1.0);
    fofGainSlider.setValue(1.0);
    fofGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
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
    bpGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
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
    fricativeGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
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
    g.fillAll (juce::Colours::black);
}

void FormantSynthAudioProcessorEditor::resized()
{
    // GUI Area variables
    const int objectBorder = 5;
    const int objectHeight = 40;
    const int keyboardHeight = 60;
    const int headerHeight = 10;
    auto area = getLocalBounds();
    auto headerArea = area.removeFromTop(headerHeight);
    auto keyboardArea = area.removeFromBottom(keyboardHeight);
    auto sourceArea = area.removeFromLeft(juce::jmax(100, getWidth() / 4));
    auto filterArea = area.removeFromLeft(juce::jmax(200, area.getWidth() /2));
    auto mixerArea = area.removeFromBottom(area.getHeight() / 4);
    auto envelopeHeaderArea = area.removeFromBottom(area.getHeight() / 2);
    auto envelopeArea = envelopeHeaderArea.removeFromBottom(envelopeHeaderArea.getHeight() - headerHeight);
    auto vibratoArea = area;

    const int sliderLeft = 80;

    keyboardComponent.setBounds(keyboardArea);
    guiGraphic.setBounds(filterArea);

    bpSourceLabel.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourceWaveBox.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourcePwSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourcePressureSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourceT0Slider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourceTeSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    bpSourceNoiseSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    fricativeSourceLabel.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    fricativeColourSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));

    voiceEnvelopeLabel.setBounds(envelopeHeaderArea.removeFromLeft(envelopeHeaderArea.getWidth() / 2));
    fricativeEnvelopeLabel.setBounds(envelopeHeaderArea);

    voiceAttackSlider.setBounds(envelopeArea.removeFromLeft(envelopeArea.getWidth() / 9).reduced(objectBorder));
    voiceDecaySlider.setBounds(envelopeArea.removeFromLeft(envelopeArea.getWidth() / 8).reduced(objectBorder));
    voiceSustainSlider.setBounds(envelopeArea.removeFromLeft(envelopeArea.getWidth() / 7).reduced(objectBorder));
    voiceReleaseSlider.setBounds(envelopeArea.removeFromLeft(envelopeArea.getWidth() / 6).reduced(objectBorder));

    fofGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight()/3).reduced(objectBorder));
    bpGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight() / 2).reduced(objectBorder));
    fricativeGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight()).reduced(objectBorder));

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