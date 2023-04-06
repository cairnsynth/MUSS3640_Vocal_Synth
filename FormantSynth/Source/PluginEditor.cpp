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
    setSize (900, 550);
    setResizable(true, false);

    /*GUI Initialisation*/


    

    textLookAndFeel.setColour(juce::Label::textColourId, juce::Colours::darkgrey);

    windowLookAndFeel.setColour(juce::TextButton::buttonColourId, juce::Colours::gainsboro);

    sourceLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::coral);
    sourceLookAndFeel.setColour(juce::Slider::backgroundColourId, sliderTrackFore);
    sourceLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colours::grey);
    sourceLookAndFeel.setColour(juce::ComboBox::backgroundColourId, sliderTrackFore);
    sourceLookAndFeel.setColour(juce::ComboBox::buttonColourId, sliderTrackFore);
    sourceLookAndFeel.setColour(juce::ComboBox::textColourId, juce::Colours::darkgrey);

    fricativeLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::mediumturquoise);
    fricativeLookAndFeel.setColour(juce::Slider::backgroundColourId, sliderTrackFore);
    fricativeLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colours::grey);

    mixerLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::darkviolet);
    mixerLookAndFeel.setColour(juce::Slider::backgroundColourId, sliderTrackFore);
    mixerLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colours::grey);

    addAndMakeVisible(&headerWindow);
    headerWindow.setLookAndFeel(&windowLookAndFeel);
    headerWindow.setButtonText("");
    headerWindow.setEnabled(false);

    addAndMakeVisible(&sourceWindow);
    sourceWindow.setLookAndFeel(&windowLookAndFeel);
    sourceWindow.setButtonText("");
    sourceWindow.setEnabled(false);

    addAndMakeVisible(&filterWindow);
    filterWindow.setLookAndFeel(&windowLookAndFeel);
    filterWindow.setButtonText("");
    filterWindow.setEnabled(false);

    addAndMakeVisible(&vibratoWindow);
    vibratoWindow.setLookAndFeel(&windowLookAndFeel);
    vibratoWindow.setButtonText("");
    vibratoWindow.setEnabled(false);

    addAndMakeVisible(&envelopeWindow);
    envelopeWindow.setLookAndFeel(&windowLookAndFeel);
    envelopeWindow.setButtonText("");
    envelopeWindow.setEnabled(false);

    addAndMakeVisible(&mixerWindow);
    mixerWindow.setLookAndFeel(&windowLookAndFeel);
    mixerWindow.setButtonText("");
    mixerWindow.setEnabled(false);

    // Keyboard
    addAndMakeVisible(&keyboardComponent);
    keyboardState.addListener(this);
    // Bandpass source label
    addAndMakeVisible(&bpSourceLabel);
    bpSourceLabel.setText("Bandpass Source", juce::dontSendNotification);
    bpSourceLabel.setLookAndFeel(&textLookAndFeel);
    // Fricative source label
    addAndMakeVisible(&fricativeSourceLabel);
    fricativeSourceLabel.setText("Fricative Source", juce::dontSendNotification);
    fricativeSourceLabel.setLookAndFeel(&textLookAndFeel);
    // Bandpass source selection box
    addAndMakeVisible(&bpSourceWaveBox);
    bpSourceWaveBox.addItemList({ "Square", "Sawtooth", "Glottal Model" }, 1);
    bpSourceWaveBox.setLookAndFeel(&sourceLookAndFeel);
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
    bpSourcePwSlider.setLookAndFeel(&sourceLookAndFeel);
    bpSourcePwSlider.onValueChange = [this] {
        audioProcessor.setBpSourcePw(bpSourcePwSlider.getValue());
    };
    addAndMakeVisible(&bpSourcePwLabel);
    bpSourcePwLabel.setText("Pulsewidth", juce::dontSendNotification);
    bpSourcePwLabel.attachToComponent(&bpSourcePwSlider, false);
    bpSourcePwLabel.setLookAndFeel(&textLookAndFeel);
    // Bandpass source pressure (glottal model)
    addAndMakeVisible(&bpSourcePressureSlider);
    bpSourcePressureSlider.setRange(0.0, 1.0);
    bpSourcePressureSlider.setValue(1.0);
    bpSourcePressureSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourcePressureSlider.setLookAndFeel(&sourceLookAndFeel);
    bpSourcePressureSlider.onValueChange = [this] {
        audioProcessor.setBpSourcePressure(bpSourcePressureSlider.getValue());
    };
    addAndMakeVisible(&bpSourcePressureLabel);
    bpSourcePressureLabel.setText("Pressure", juce::dontSendNotification);
    bpSourcePressureLabel.attachToComponent(&bpSourcePressureSlider, false);
    bpSourcePressureLabel.setLookAndFeel(&textLookAndFeel);
    // Bandpass source T0 (glottal model)
    addAndMakeVisible(&bpSourceT0Slider);
    bpSourceT0Slider.setRange(0.0, 1.0);
    bpSourceT0Slider.setValue(0.944);
    bpSourceT0Slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceT0Slider.setLookAndFeel(&sourceLookAndFeel);
    bpSourceT0Slider.onValueChange = [this] {
        audioProcessor.setBpSourceT0(bpSourceT0Slider.getValue());
    };
    addAndMakeVisible(&bpSourceT0Label);
    bpSourceT0Label.setText("T0", juce::dontSendNotification);
    bpSourceT0Label.attachToComponent(&bpSourceT0Slider, false);
    bpSourceT0Label.setLookAndFeel(&textLookAndFeel);
    // Bandpass source Te (glottal model)
    addAndMakeVisible(&bpSourceTeSlider);
    bpSourceTeSlider.setRange(0.0, 1.0);
    bpSourceTeSlider.setValue(0.224);
    bpSourceTeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceTeSlider.setLookAndFeel(&sourceLookAndFeel);
    bpSourceTeSlider.onValueChange = [this] {
        audioProcessor.setBpSourceTe(bpSourceTeSlider.getValue());
    };
    addAndMakeVisible(&bpSourceTeLabel);
    bpSourceTeLabel.setText("Te", juce::dontSendNotification);
    bpSourceTeLabel.attachToComponent(&bpSourceTeSlider, false);
    bpSourceTeLabel.setLookAndFeel(&textLookAndFeel);
    // Bandpass source noise (glottal model)
    addAndMakeVisible(&bpSourceNoiseSlider);
    bpSourceNoiseSlider.setRange(0.0, 1.0);
    bpSourceNoiseSlider.setValue(0.0);
    bpSourceNoiseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceNoiseSlider.setLookAndFeel(&sourceLookAndFeel);
    bpSourceNoiseSlider.onValueChange = [this] {
        audioProcessor.setBpSourceNoise(bpSourceNoiseSlider.getValue());
    };
    addAndMakeVisible(&bpSourceNoiseLabel);
    bpSourceNoiseLabel.setText("Noise", juce::dontSendNotification);
    bpSourceNoiseLabel.attachToComponent(&bpSourceNoiseSlider, false);
    bpSourceNoiseLabel.setLookAndFeel(&textLookAndFeel);
    // Fricative colour slider
    addAndMakeVisible(&fricativeColourSlider);
    fricativeColourSlider.setSliderStyle(juce::Slider::TwoValueHorizontal);
    fricativeColourSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeColourSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeColourSlider.setRange(20.0, 20000.0);
    fricativeColourSlider.setMinAndMaxValues(20.0, 20000.0, juce::dontSendNotification);
    fricativeColourSlider.onValueChange = [this] {
        audioProcessor.setFricativeColour(fricativeColourSlider.getMinValue(), fricativeColourSlider.getMaxValue());
    };

    // Voice envelope label
    addAndMakeVisible(&voiceEnvelopeLabel);
    voiceEnvelopeLabel.setText("Voice", juce::dontSendNotification);
    voiceEnvelopeLabel.setLookAndFeel(&textLookAndFeel);
    // Voice attack
    addAndMakeVisible(&voiceAttackSlider);
    voiceAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceAttackSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceAttackSlider.setRange(0.0, 2.0);
    voiceAttackSlider.setValue(0.05);
    voiceAttackSlider.onValueChange = [this] {
        audioProcessor.setVoiceAttack(voiceAttackSlider.getValue());
    };
    addAndMakeVisible(&voiceAttackLabel);
    voiceAttackLabel.setText("A", juce::dontSendNotification);
    voiceAttackLabel.attachToComponent(&voiceAttackSlider, false);
    voiceAttackLabel.setLookAndFeel(&textLookAndFeel);
    // Voice decay
    addAndMakeVisible(&voiceDecaySlider);
    voiceDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceDecaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceDecaySlider.setLookAndFeel(&sourceLookAndFeel);
    voiceDecaySlider.setRange(0.0, 2.0);
    voiceDecaySlider.setValue(0.05);
    voiceDecaySlider.onValueChange = [this] {
        audioProcessor.setVoiceDecay(voiceDecaySlider.getValue());
    };
    addAndMakeVisible(&voiceDecayLabel);
    voiceDecayLabel.setText("D", juce::dontSendNotification);
    voiceDecayLabel.attachToComponent(&voiceDecaySlider, false);
    voiceDecayLabel.setLookAndFeel(&textLookAndFeel);
    // Voice sustain
    addAndMakeVisible(&voiceSustainSlider);
    voiceSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceSustainSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceSustainSlider.setRange(0.0, 1.0);
    voiceSustainSlider.setValue(1.0);
    voiceSustainSlider.onValueChange = [this] {
        audioProcessor.setVoiceSustain(voiceSustainSlider.getValue());
    };
    addAndMakeVisible(&voiceSustainLabel);
    voiceSustainLabel.setText("S", juce::dontSendNotification);
    voiceSustainLabel.attachToComponent(&voiceSustainSlider, false);
    voiceSustainLabel.setLookAndFeel(&textLookAndFeel);
    // Voice release
    addAndMakeVisible(&voiceReleaseSlider);
    voiceReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceReleaseSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceReleaseSlider.setRange(0.0, 4.0);
    voiceReleaseSlider.setValue(1.0);
    voiceReleaseSlider.onValueChange = [this] {
        audioProcessor.setVoiceRelease(voiceReleaseSlider.getValue());
    };
    addAndMakeVisible(&voiceReleaseLabel);
    voiceReleaseLabel.setText("R", juce::dontSendNotification);
    voiceReleaseLabel.attachToComponent(&voiceReleaseSlider, false);
    voiceReleaseLabel.setLookAndFeel(&textLookAndFeel);
    // Fricative envelope label
    addAndMakeVisible(&fricativeEnvelopeLabel);
    fricativeEnvelopeLabel.setText("Fricative", juce::dontSendNotification);
    fricativeEnvelopeLabel.setLookAndFeel(&textLookAndFeel);
    // Fricative attack
    addAndMakeVisible(&fricativeAttackSlider);
    fricativeAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeAttackSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeAttackSlider.setRange(0.0, 2.0);
    fricativeAttackSlider.setValue(0.01);
    fricativeAttackSlider.onValueChange = [this] {
        audioProcessor.setFricativeAttack(fricativeAttackSlider.getValue());
    };
    addAndMakeVisible(&fricativeAttackLabel);
    fricativeAttackLabel.setText("A", juce::dontSendNotification);
    fricativeAttackLabel.attachToComponent(&fricativeAttackSlider, false);
    fricativeAttackLabel.setLookAndFeel(&textLookAndFeel);
    // Fricative decay
    addAndMakeVisible(&fricativeDecaySlider);
    fricativeDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeDecaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeDecaySlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeDecaySlider.setRange(0.0, 2.0);
    fricativeDecaySlider.setValue(0.05);
    fricativeDecaySlider.onValueChange = [this] {
        audioProcessor.setFricativeDecay(fricativeDecaySlider.getValue());
    };
    addAndMakeVisible(&fricativeDecayLabel);
    fricativeDecayLabel.setText("D", juce::dontSendNotification);
    fricativeDecayLabel.attachToComponent(&fricativeDecaySlider, false);
    fricativeDecayLabel.setLookAndFeel(&textLookAndFeel);
    // Fricative sustain
    addAndMakeVisible(&fricativeSustainSlider);
    fricativeSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeSustainSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeSustainSlider.setRange(0.0, 1.0);
    fricativeSustainSlider.setValue(0.0);
    fricativeSustainSlider.onValueChange = [this] {
        audioProcessor.setFricativeSustain(fricativeSustainSlider.getValue());
    };
    addAndMakeVisible(&fricativeSustainLabel);
    fricativeSustainLabel.setText("S", juce::dontSendNotification);
    fricativeSustainLabel.attachToComponent(&fricativeSustainSlider, false);
    fricativeSustainLabel.setLookAndFeel(&textLookAndFeel);
    // Fricative release
    addAndMakeVisible(&fricativeReleaseSlider);
    fricativeReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeReleaseSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeReleaseSlider.setRange(0.0, 4.0);
    fricativeReleaseSlider.setValue(1.0);
    fricativeReleaseSlider.onValueChange = [this] {
        audioProcessor.setFricativeRelease(fricativeReleaseSlider.getValue());
    };
    addAndMakeVisible(&fricativeReleaseLabel);
    fricativeReleaseLabel.setText("R", juce::dontSendNotification);
    fricativeReleaseLabel.attachToComponent(&fricativeReleaseSlider, false);
    fricativeReleaseLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato label
    addAndMakeVisible(&vibratoLabel);
    vibratoLabel.setText("Vibrato", juce::dontSendNotification);
    vibratoLabel.setLookAndFeel(&textLookAndFeel);
    // Vibrato frequency
    addAndMakeVisible(&vibratoFrequencySlider);
    vibratoFrequencySlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoFrequencySlider.setRange(0.0, 8.0);
    vibratoFrequencySlider.setValue(5.0);
    vibratoFrequencySlider.onValueChange = [this] {
        audioProcessor.setVibratoFrequency(vibratoFrequencySlider.getValue());
    };
    addAndMakeVisible(&vibratoFrequencyLabel);
    vibratoFrequencyLabel.setText("Freq", juce::dontSendNotification);
    vibratoFrequencyLabel.attachToComponent(&vibratoFrequencySlider, false);
    vibratoFrequencyLabel.setLookAndFeel(&textLookAndFeel);
    // Vibrato attack
    addAndMakeVisible(&vibratoAttackSlider);
    vibratoAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoAttackSlider.setRange(0.0, 6.0);
    vibratoAttackSlider.setValue(1.0);
    vibratoAttackSlider.onValueChange = [this] {
        audioProcessor.setVibratoAttack(vibratoAttackSlider.getValue());
    };
    addAndMakeVisible(&vibratoAttackLabel);
    vibratoAttackLabel.setText("A", juce::dontSendNotification);
    vibratoAttackLabel.attachToComponent(&vibratoAttackSlider, false);
    vibratoAttackLabel.setLookAndFeel(&textLookAndFeel);
    // Vibrato sustain
    addAndMakeVisible(&vibratoSustainSlider);
    vibratoSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoSustainSlider.setRange(0.0, 1.0);
    vibratoSustainSlider.setValue(1.0);
    vibratoSustainSlider.onValueChange = [this] {
        audioProcessor.setVibratoSustain(vibratoSustainSlider.getValue());
    };
    addAndMakeVisible(&vibratoSustainLabel);
    vibratoSustainLabel.setText("S", juce::dontSendNotification);
    vibratoSustainLabel.attachToComponent(&vibratoSustainSlider, false);
    vibratoSustainLabel.setLookAndFeel(&textLookAndFeel);
    // Vibrato release
    addAndMakeVisible(&vibratoReleaseSlider);
    vibratoReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoReleaseSlider.setRange(0.0, 6.0);
    vibratoReleaseSlider.setValue(1.0);
    vibratoReleaseSlider.onValueChange = [this] {
        audioProcessor.setVibratoRelease(vibratoReleaseSlider.getValue());
    };
    addAndMakeVisible(&vibratoReleaseLabel);
    vibratoReleaseLabel.setText("R", juce::dontSendNotification);
    vibratoReleaseLabel.attachToComponent(&vibratoReleaseSlider, false);
    vibratoReleaseLabel.setLookAndFeel(&textLookAndFeel);
    // FOF Gain
    addAndMakeVisible(&fofGainSlider);
    fofGainSlider.setRange(0.0, 1.0);
    fofGainSlider.setValue(1.0);
    fofGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fofGainSlider.setLookAndFeel(&mixerLookAndFeel);
    fofGainSlider.onValueChange = [this] {
        audioProcessor.setFofGain(fofGainSlider.getValue());
    };
    addAndMakeVisible(&fofGainLabel);
    fofGainLabel.setText("FOF Gain", juce::dontSendNotification);
    fofGainLabel.attachToComponent(&fofGainSlider, false);
    fofGainLabel.setLookAndFeel(&textLookAndFeel);
    // BP Gain
    addAndMakeVisible(&bpGainSlider);
    bpGainSlider.setRange(0.0, 1.0);
    bpGainSlider.setValue(1.0);
    bpGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpGainSlider.setLookAndFeel(&mixerLookAndFeel);
    bpGainSlider.onValueChange = [this] {
        audioProcessor.setBpGain(bpGainSlider.getValue());
    };
    addAndMakeVisible(&bpGainLabel);
    bpGainLabel.setText("BP Gain", juce::dontSendNotification);
    bpGainLabel.attachToComponent(&bpGainSlider, false);
    bpGainLabel.setLookAndFeel(&textLookAndFeel);
    // Fricative Gain
    addAndMakeVisible(&fricativeGainSlider);
    fricativeGainSlider.setRange(0.0, 1.0);
    fricativeGainSlider.setValue(1.0);
    fricativeGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeGainSlider.setLookAndFeel(&mixerLookAndFeel);
    fricativeGainSlider.onValueChange = [this] {
        audioProcessor.setFricativeGain(fricativeGainSlider.getValue());
    };
    addAndMakeVisible(&fricativeGainLabel);
    fricativeGainLabel.setText("Fricative Gain", juce::dontSendNotification);
    fricativeGainLabel.attachToComponent(&fricativeGainSlider, false);
    fricativeGainLabel.setLookAndFeel(&textLookAndFeel);

}

FormantSynthAudioProcessorEditor::~FormantSynthAudioProcessorEditor()
{
    keyboardState.removeListener(this);
}

//==============================================================================
void FormantSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::grey);
}

void FormantSynthAudioProcessorEditor::resized()
{
    // GUI Area variables
    const int objectBorder = 10;
    const int windowBorder = 2;
    const int objectHeight = 40;
    const int keyboardHeight = 60;
    const int headerHeight = 10;
    auto area = getLocalBounds();
    auto headerArea = area.removeFromTop(headerHeight);
    auto keyboardArea = area.removeFromBottom(keyboardHeight);
    auto sourceArea = area.removeFromLeft(juce::jmax(100, getWidth() / 4));
    auto filterArea = area.removeFromLeft(juce::jmax(200, area.getWidth() /2));
    auto mixerArea = area.removeFromBottom(area.getHeight() / 4);
    auto envelopeArea = area.removeFromBottom(area.getHeight() / 2);
    auto vibratoArea = area;


    keyboardComponent.setBounds(keyboardArea);
    headerWindow.setBounds(headerArea.reduced(windowBorder));
    sourceWindow.setBounds(sourceArea.reduced(windowBorder));
    filterWindow.setBounds(filterArea.reduced(windowBorder));
    vibratoWindow.setBounds(vibratoArea.reduced(windowBorder));
    envelopeWindow.setBounds(envelopeArea.reduced(windowBorder));
    mixerWindow.setBounds(mixerArea.reduced(windowBorder));

    auto envelopeHeaderArea = envelopeArea.removeFromTop(headerHeight);

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
    envelopeArea.removeFromLeft(envelopeArea.getWidth() / 5);
    fricativeAttackSlider.setBounds(envelopeArea.removeFromLeft(envelopeArea.getWidth() / 4).reduced(objectBorder));
    fricativeDecaySlider.setBounds(envelopeArea.removeFromLeft(envelopeArea.getWidth() / 3).reduced(objectBorder));
    fricativeSustainSlider.setBounds(envelopeArea.removeFromLeft(envelopeArea.getWidth() / 2).reduced(objectBorder));
    fricativeReleaseSlider.setBounds(envelopeArea.removeFromLeft(envelopeArea.getWidth()).reduced(objectBorder));

    fofGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight()/3).reduced(objectBorder));
    bpGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight() / 2).reduced(objectBorder));
    fricativeGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight()).reduced(objectBorder));

    vibratoLabel.setBounds(vibratoArea.removeFromTop(headerHeight));
    vibratoFrequencySlider.setBounds(vibratoArea.removeFromLeft(vibratoArea.getWidth() / 5).reduced(objectBorder));
    vibratoArea.removeFromLeft(vibratoArea.getWidth() / 4);
    vibratoAttackSlider.setBounds(vibratoArea.removeFromLeft(vibratoArea.getWidth() / 3).reduced(objectBorder));
    vibratoSustainSlider.setBounds(vibratoArea.removeFromLeft(vibratoArea.getWidth() / 2).reduced(objectBorder));
    vibratoReleaseSlider.setBounds(vibratoArea.removeFromLeft(vibratoArea.getWidth()).reduced(objectBorder));

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