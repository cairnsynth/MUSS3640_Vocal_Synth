#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FormantSynthAudioProcessorEditor::FormantSynthAudioProcessorEditor (FormantSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Audio Processor Value Tree attachments
    

    setSize (900, 550);
    setResizable(true, false);

    /*GUI Initialisation*/
    textLookAndFeel.setColour(juce::Label::textColourId, juce::Colours::darkgrey);

    windowLookAndFeel.setColour(juce::TextButton::buttonColourId, juce::Colours::gainsboro);

    sourceLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::coral);
    sourceLookAndFeel.setColour(juce::Slider::backgroundColourId, sliderTrackFore);
    sourceLookAndFeel.setColour(juce::Slider::trackColourId, sliderTrackBack);
    sourceLookAndFeel.setColour(juce::ComboBox::backgroundColourId, sliderTrackFore);
    sourceLookAndFeel.setColour(juce::ComboBox::buttonColourId, sliderTrackFore);
    sourceLookAndFeel.setColour(juce::ComboBox::textColourId, juce::Colours::darkgrey);
    sourceLookAndFeel.setColour(juce::ComboBox::arrowColourId, juce::Colours::coral);
    sourceLookAndFeel.setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, juce::Colours::coral);
    
    fricativeLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::mediumturquoise);
    fricativeLookAndFeel.setColour(juce::Slider::backgroundColourId, sliderTrackFore);
    fricativeLookAndFeel.setColour(juce::Slider::trackColourId, sliderTrackBack);

    mixerLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::darkviolet);
    mixerLookAndFeel.setColour(juce::Slider::backgroundColourId, sliderTrackFore);
    mixerLookAndFeel.setColour(juce::Slider::trackColourId, sliderTrackBack);

    disabledLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::dimgrey);
    disabledLookAndFeel.setColour(juce::Slider::backgroundColourId, sliderTrackFore);
    disabledLookAndFeel.setColour(juce::Slider::trackColourId, sliderTrackBack);

    // GUI windows
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
    keyboardComponent.setLookAndFeel(&sourceLookAndFeel);

    // Bandpass source label
    addAndMakeVisible(&bpSourceLabel);
    bpSourceLabel.setText("Bandpass Source", juce::dontSendNotification);
    bpSourceLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative source label
    addAndMakeVisible(&fricativeSourceLabel);
    fricativeSourceLabel.setText("Fricative Source", juce::dontSendNotification);
    fricativeSourceLabel.setLookAndFeel(&textLookAndFeel);

    int count = 0;

    // Bandpass source selection box
    bpSourceWaveComboAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (audioProcessor.apvts, SOURCE_WAVE_ID, bpSourceWaveBox);
    count++;
    DBG(count);
    addAndMakeVisible(&bpSourceWaveBox);
    bpSourceWaveBox.addItemList({ "Square", "Sawtooth", "Glottal Model" }, 1);
    bpSourceWaveBox.setLookAndFeel(&sourceLookAndFeel);
    bpSourceWaveBox.onChange = [this] {
        enableSourceGui(bpSourceWaveBox.getSelectedId());
    };
    bpSourceWaveBox.setSelectedItemIndex(0);

    // Bandpass source pulsewidth (square)
    bpSourcePwSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_PW_ID, bpSourcePwSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&bpSourcePwSlider);
    bpSourcePwSlider.setRange(0.0, 1.0);
    bpSourcePwSlider.setValue(0.5);
    bpSourcePwSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourcePwSlider.setLookAndFeel(&sourceLookAndFeel);

    addAndMakeVisible(&bpSourcePwLabel);
    bpSourcePwLabel.setText("Pulsewidth", juce::dontSendNotification);
    //bpSourcePwLabel.attachToComponent(&bpSourcePwSlider, true);
    bpSourcePwLabel.setLookAndFeel(&textLookAndFeel);

    // Bandpass source pressure (glottal model)
    bpSourcePressureAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_PRESSURE_ID, bpSourcePressureSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&bpSourcePressureSlider);
    bpSourcePressureSlider.setRange(0.0, 1.0);
    bpSourcePressureSlider.setValue(1.0);
    bpSourcePressureSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourcePressureSlider.setLookAndFeel(&sourceLookAndFeel);
   
    addAndMakeVisible(&bpSourcePressureLabel);
    bpSourcePressureLabel.setText("Pressure", juce::dontSendNotification);
    //bpSourcePressureLabel.attachToComponent(&bpSourcePressureSlider, false);
    bpSourcePressureLabel.setLookAndFeel(&textLookAndFeel);

    // Bandpass source T0 (glottal model)
    bpSourceT0Attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_T0_ID, bpSourceT0Slider);
    count++;
    DBG(count);
    addAndMakeVisible(&bpSourceT0Slider);
    bpSourceT0Slider.setRange(0.0, 1.0);
    bpSourceT0Slider.setValue(0.944);
    bpSourceT0Slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceT0Slider.setLookAndFeel(&sourceLookAndFeel);

    addAndMakeVisible(&bpSourceT0Label);
    bpSourceT0Label.setText("T0", juce::dontSendNotification);
    //bpSourceT0Label.attachToComponent(&bpSourceT0Slider, false);
    bpSourceT0Label.setLookAndFeel(&textLookAndFeel);

    // Bandpass source Te (glottal model)
    bpSourceTeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_TE_ID, bpSourceTeSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&bpSourceTeSlider);
    bpSourceTeSlider.setRange(0.0, 1.0);
    bpSourceTeSlider.setValue(0.224);
    bpSourceTeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceTeSlider.setLookAndFeel(&sourceLookAndFeel);
    
    addAndMakeVisible(&bpSourceTeLabel);
    bpSourceTeLabel.setText("Te", juce::dontSendNotification);
    //bpSourceTeLabel.attachToComponent(&bpSourceTeSlider, false);
    bpSourceTeLabel.setLookAndFeel(&textLookAndFeel);

    // Bandpass source noise (glottal model)
    bpSourceNoiseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_NOISE_ID, bpSourceNoiseSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&bpSourceNoiseSlider);
    bpSourceNoiseSlider.setRange(0.0, 1.0);
    bpSourceNoiseSlider.setValue(0.0);
    bpSourceNoiseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceNoiseSlider.setLookAndFeel(&sourceLookAndFeel);
    
    addAndMakeVisible(&bpSourceNoiseLabel);
    bpSourceNoiseLabel.setText("Noise", juce::dontSendNotification);
    //bpSourceNoiseLabel.attachToComponent(&bpSourceNoiseSlider, false);
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

    // Mono / Poly select
    monoAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, MONO_ID, monoButton);
    addAndMakeVisible(&monoButton);

    /*Filter*/
    // F1 frequency
    addAndMakeVisible(&f1FreqSlider);
    f1FreqSlider.setSliderStyle(juce::Slider::Rotary);
    f1FreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    f1FreqSlider.setRange(100.0, 5000.0);
    f1FreqSlider.setValue(800.0);
    f1FreqSlider.onValueChange = [this] {
        audioProcessor.setF1Freq(f1FreqSlider.getValue());
    };
    // F1 bandwidth
    addAndMakeVisible(&f1BandwidthSlider);
    f1BandwidthSlider.setSliderStyle(juce::Slider::Rotary);
    f1BandwidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    f1BandwidthSlider.setRange(0.0, 1000.0);
    f1BandwidthSlider.setValue(80.0);
    f1BandwidthSlider.onValueChange = [this] {
        audioProcessor.setF1Bandwidth(f1BandwidthSlider.getValue());
    };
    // F1 gain
    addAndMakeVisible(&f1GainSlider);
    f1GainSlider.setSliderStyle(juce::Slider::Rotary);
    f1GainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    f1GainSlider.setRange(0.0, 1.0);
    f1GainSlider.setValue(1.0);
    f1GainSlider.onValueChange = [this] {
        audioProcessor.setF1Gain(f1GainSlider.getValue());
    };

    // Voice envelope label
    addAndMakeVisible(&voiceEnvelopeLabel);
    voiceEnvelopeLabel.setText("Voice", juce::dontSendNotification);
    voiceEnvelopeLabel.setLookAndFeel(&textLookAndFeel);

    // Voice attack
    voiceAttachAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VOICE_ATTACK_ID, voiceAttackSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&voiceAttackSlider);
    voiceAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceAttackSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceAttackSlider.setRange(0.0, 2.0);
    voiceAttackSlider.setValue(0.05);
    
    addAndMakeVisible(&voiceAttackLabel);
    voiceAttackLabel.setText("A", juce::dontSendNotification);
    voiceAttackLabel.attachToComponent(&voiceAttackSlider, false);
    voiceAttackLabel.setLookAndFeel(&textLookAndFeel);

    // Voice decay
    voiceDecayAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VOICE_DECAY_ID, voiceDecaySlider);
    count++;
    DBG(count);
    addAndMakeVisible(&voiceDecaySlider);
    voiceDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceDecaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceDecaySlider.setLookAndFeel(&sourceLookAndFeel);
    voiceDecaySlider.setRange(0.0, 2.0);
    voiceDecaySlider.setValue(0.05);
    
    addAndMakeVisible(&voiceDecayLabel);
    voiceDecayLabel.setText("D", juce::dontSendNotification);
    voiceDecayLabel.attachToComponent(&voiceDecaySlider, false);
    voiceDecayLabel.setLookAndFeel(&textLookAndFeel);

    // Voice sustain
    voiceSustainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VOICE_SUSTAIN_ID, voiceSustainSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&voiceSustainSlider);
    voiceSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceSustainSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceSustainSlider.setRange(0.0, 1.0);
    voiceSustainSlider.setValue(1.0);
    
    addAndMakeVisible(&voiceSustainLabel);
    voiceSustainLabel.setText("S", juce::dontSendNotification);
    voiceSustainLabel.attachToComponent(&voiceSustainSlider, false);
    voiceSustainLabel.setLookAndFeel(&textLookAndFeel);

    // Voice release
    voiceReleaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VOICE_RELEASE_ID, voiceReleaseSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&voiceReleaseSlider);
    voiceReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceReleaseSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceReleaseSlider.setRange(0.0, 4.0);
    voiceReleaseSlider.setValue(1.0);
    
    addAndMakeVisible(&voiceReleaseLabel);
    voiceReleaseLabel.setText("R", juce::dontSendNotification);
    voiceReleaseLabel.attachToComponent(&voiceReleaseSlider, false);
    voiceReleaseLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative envelope label
    addAndMakeVisible(&fricativeEnvelopeLabel);
    fricativeEnvelopeLabel.setText("Fricative", juce::dontSendNotification);
    fricativeEnvelopeLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative attack
    fricativeAttackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_ATTACK_ID, fricativeAttackSlider);

    count++;
    DBG(count);
    addAndMakeVisible(&fricativeAttackSlider);
    fricativeAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeAttackSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeAttackSlider.setRange(0.0, 2.0);
    fricativeAttackSlider.setValue(0.01);
    
    addAndMakeVisible(&fricativeAttackLabel);
    fricativeAttackLabel.setText("A", juce::dontSendNotification);
    fricativeAttackLabel.attachToComponent(&fricativeAttackSlider, false);
    fricativeAttackLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative decay
    fricativeDecayAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_DECAY_ID, fricativeDecaySlider);
    count++;
    DBG(count);
    addAndMakeVisible(&fricativeDecaySlider);
    fricativeDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeDecaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeDecaySlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeDecaySlider.setRange(0.0, 2.0);
    fricativeDecaySlider.setValue(0.05);
    
    addAndMakeVisible(&fricativeDecayLabel);
    fricativeDecayLabel.setText("D", juce::dontSendNotification);
    fricativeDecayLabel.attachToComponent(&fricativeDecaySlider, false);
    fricativeDecayLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative sustain
    fricativeSustainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_SUSTAIN_ID, fricativeSustainSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&fricativeSustainSlider);
    fricativeSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeSustainSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeSustainSlider.setRange(0.0, 1.0);
    fricativeSustainSlider.setValue(0.0);
    
    addAndMakeVisible(&fricativeSustainLabel);
    fricativeSustainLabel.setText("S", juce::dontSendNotification);
    fricativeSustainLabel.attachToComponent(&fricativeSustainSlider, false);
    fricativeSustainLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative release
    fricativeReleaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_RELEASE_ID, fricativeReleaseSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&fricativeReleaseSlider);
    fricativeReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeReleaseSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeReleaseSlider.setRange(0.0, 4.0);
    fricativeReleaseSlider.setValue(1.0);
    
    addAndMakeVisible(&fricativeReleaseLabel);
    fricativeReleaseLabel.setText("R", juce::dontSendNotification);
    fricativeReleaseLabel.attachToComponent(&fricativeReleaseSlider, false);
    fricativeReleaseLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato label
    addAndMakeVisible(&vibratoLabel);
    vibratoLabel.setText("Vibrato", juce::dontSendNotification);
    vibratoLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato frequency
    vibratoFrequencyAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VIBRATO_FREQUENCY_ID, vibratoFrequencySlider);
    count++;
    DBG(count);
    addAndMakeVisible(&vibratoFrequencySlider);
    vibratoFrequencySlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoFrequencySlider.setRange(0.0, 8.0);
    vibratoFrequencySlider.setValue(5.0);
    
    addAndMakeVisible(&vibratoFrequencyLabel);
    vibratoFrequencyLabel.setText("Freq", juce::dontSendNotification);
    vibratoFrequencyLabel.attachToComponent(&vibratoFrequencySlider, false);
    vibratoFrequencyLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato attack
    vibratoAttackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VIBRATO_ATTACK_ID, vibratoAttackSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&vibratoAttackSlider);
    vibratoAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoAttackSlider.setRange(0.0, 6.0);
    vibratoAttackSlider.setValue(1.0);
    
    addAndMakeVisible(&vibratoAttackLabel);
    vibratoAttackLabel.setText("A", juce::dontSendNotification);
    vibratoAttackLabel.attachToComponent(&vibratoAttackSlider, false);
    vibratoAttackLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato sustain
    vibratoSustainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VIBRATO_SUSTAIN_ID, vibratoSustainSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&vibratoSustainSlider);
    vibratoSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoSustainSlider.setRange(0.0, 1.0);
    vibratoSustainSlider.setValue(1.0);
    
    addAndMakeVisible(&vibratoSustainLabel);
    vibratoSustainLabel.setText("S", juce::dontSendNotification);
    vibratoSustainLabel.attachToComponent(&vibratoSustainSlider, false);
    vibratoSustainLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato release
    vibratoReleaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VIBRATO_RELEASE_ID, vibratoReleaseSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&vibratoReleaseSlider);
    vibratoReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoReleaseSlider.setRange(0.0, 6.0);
    vibratoReleaseSlider.setValue(1.0);
    
    addAndMakeVisible(&vibratoReleaseLabel);
    vibratoReleaseLabel.setText("R", juce::dontSendNotification);
    vibratoReleaseLabel.attachToComponent(&vibratoReleaseSlider, false);
    vibratoReleaseLabel.setLookAndFeel(&textLookAndFeel);

    // FOF Gain
    fofGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FOF_GAIN_ID, fofGainSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&fofGainSlider);
    fofGainSlider.setRange(0.0, 1.0);
    fofGainSlider.setValue(1.0);
    fofGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fofGainSlider.setLookAndFeel(&mixerLookAndFeel);
    
    addAndMakeVisible(&fofGainLabel);
    fofGainLabel.setText("FOF Gain", juce::dontSendNotification);
    fofGainLabel.attachToComponent(&fofGainSlider, false);
    fofGainLabel.setLookAndFeel(&textLookAndFeel);

    // BP Gain
    bpGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, BP_GAIN_ID, bpGainSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&bpGainSlider);
    bpGainSlider.setRange(0.0, 1.0);
    bpGainSlider.setValue(1.0);
    bpGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpGainSlider.setLookAndFeel(&mixerLookAndFeel);
    
    addAndMakeVisible(&bpGainLabel);
    bpGainLabel.setText("BP Gain", juce::dontSendNotification);
    bpGainLabel.attachToComponent(&bpGainSlider, false);
    bpGainLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative Gain
    fricativeGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_GAIN_ID, fricativeGainSlider);
    count++;
    DBG(count);
    addAndMakeVisible(&fricativeGainSlider);
    fricativeGainSlider.setRange(0.0, 1.0);
    fricativeGainSlider.setValue(1.0);
    fricativeGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeGainSlider.setLookAndFeel(&mixerLookAndFeel);
    
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
    g.fillAll (sliderTrackFore);
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
    auto formantArea = filterArea.removeFromTop(filterArea.getHeight() / 3);
    auto mixerArea = area.removeFromBottom(area.getHeight() / 4);
    auto envelopeArea = area.removeFromBottom(area.getHeight() / 2);
    auto vibratoArea = area;

    juce::FlexBox sourceFB;
    sourceFB.flexWrap = juce::FlexBox::Wrap::wrap;
    sourceFB.justifyContent = juce::FlexBox::JustifyContent::center;
    sourceFB.alignContent = juce::FlexBox::AlignContent::center;
    sourceFB.flexDirection = juce::FlexBox::Direction::column;
    sourceFB.items.addArray({
        juce::FlexItem(bpSourceLabel).withFlex(1, 1, sourceArea.getHeight()/16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourceWaveBox).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourcePwLabel).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourcePwSlider).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourcePressureLabel).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourcePressureSlider).withFlex(1, 1, sourceArea.getHeight() /16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourceT0Label).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourceT0Slider).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourceTeLabel).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourceTeSlider).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourceNoiseLabel).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(bpSourceNoiseSlider).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(fricativeSourceLabel).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(fricativeColourSlider).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight) });
    sourceFB.performLayout(sourceArea.reduced(objectBorder).toFloat());

    monoButton.setBounds(filterArea.reduced(objectBorder));

    keyboardComponent.setBounds(keyboardArea);
    headerWindow.setBounds(headerArea.reduced(windowBorder));
    sourceWindow.setBounds(sourceArea.reduced(windowBorder));
    filterWindow.setBounds(filterArea.reduced(windowBorder));
    vibratoWindow.setBounds(vibratoArea.reduced(windowBorder));
    envelopeWindow.setBounds(envelopeArea.reduced(windowBorder));
    mixerWindow.setBounds(mixerArea.reduced(windowBorder));

    auto envelopeHeaderArea = envelopeArea.removeFromTop(headerHeight);

    //bpSourceLabel.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    //bpSourceWaveBox.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    //bpSourcePwSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    //bpSourcePressureSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    //bpSourceT0Slider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    //bpSourceTeSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    //bpSourceNoiseSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    //fricativeSourceLabel.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));
    //fricativeColourSlider.setBounds(sourceArea.removeFromTop(objectHeight).reduced(objectBorder));

    auto f1Area = formantArea.removeFromLeft(formantArea.getWidth() / 5);
    f1FreqSlider.setBounds(f1Area.removeFromTop(f1Area.getHeight() / 3).reduced(0));
    f1BandwidthSlider.setBounds(f1Area.removeFromTop(f1Area.getHeight() / 2).reduced(0));
    f1GainSlider.setBounds(f1Area.reduced(0));

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
        bpSourcePressureSlider.setEnabled(false);
        bpSourceT0Slider.setEnabled(false);
        bpSourceTeSlider.setEnabled(false);
        bpSourceNoiseSlider.setEnabled(false);

        bpSourcePressureSlider.setLookAndFeel(&disabledLookAndFeel);
        bpSourceT0Slider.setLookAndFeel(&disabledLookAndFeel);
        bpSourceTeSlider.setLookAndFeel(&disabledLookAndFeel);
        bpSourceNoiseSlider.setLookAndFeel(&disabledLookAndFeel);
        break;
    }
    case 2:
    {
        bpSourcePwSlider.setEnabled(false);
        bpSourcePressureSlider.setEnabled(false);
        bpSourceT0Slider.setEnabled(false);
        bpSourceTeSlider.setEnabled(false);
        bpSourceNoiseSlider.setEnabled(false);

        bpSourcePwSlider.setLookAndFeel(&disabledLookAndFeel);
        bpSourcePressureSlider.setLookAndFeel(&disabledLookAndFeel);
        bpSourceT0Slider.setLookAndFeel(&disabledLookAndFeel);
        bpSourceTeSlider.setLookAndFeel(&disabledLookAndFeel);
        bpSourceNoiseSlider.setLookAndFeel(&disabledLookAndFeel);
        break;
    }
    case 3:
    {
        bpSourcePwSlider.setEnabled(false);
        bpSourcePressureSlider.setEnabled(true);
        bpSourceT0Slider.setEnabled(true);
        bpSourceTeSlider.setEnabled(true);
        bpSourceNoiseSlider.setEnabled(true);

        bpSourcePwSlider.setLookAndFeel(&disabledLookAndFeel);
        bpSourcePressureSlider.setLookAndFeel(&sourceLookAndFeel);
        bpSourceT0Slider.setLookAndFeel(&sourceLookAndFeel);
        bpSourceTeSlider.setLookAndFeel(&sourceLookAndFeel);
        bpSourceNoiseSlider.setLookAndFeel(&sourceLookAndFeel);
        break;
    }
    }
}