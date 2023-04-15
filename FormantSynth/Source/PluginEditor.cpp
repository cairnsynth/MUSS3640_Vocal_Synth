#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FormantSynthAudioProcessorEditor::FormantSynthAudioProcessorEditor(FormantSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    keyboardComponent(audioProcessor.keyboardState,
    juce::MidiKeyboardComponent::horizontalKeyboard),
    table(p.midiModel)
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

    filterLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colours::transparentWhite);
    
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
    audioProcessor.keyboardState.addListener(this);
    keyboardComponent.setLookAndFeel(&sourceLookAndFeel);

    // Bandpass source label
    addAndMakeVisible(&bpSourceLabel);
    bpSourceLabel.setText("Bandpass Source", juce::dontSendNotification);
    bpSourceLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative source label
    addAndMakeVisible(&fricativeSourceLabel);
    fricativeSourceLabel.setText("Fricative Source", juce::dontSendNotification);
    fricativeSourceLabel.setLookAndFeel(&textLookAndFeel);

    // Bandpass source selection box
    bpSourceWaveComboAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (audioProcessor.apvts, SOURCE_WAVE_ID, bpSourceWaveBox);
    addAndMakeVisible(&bpSourceWaveBox);
    bpSourceWaveBox.addItemList({ "Square", "Sawtooth", "Glottal Model" }, 1);
    bpSourceWaveBox.setLookAndFeel(&sourceLookAndFeel);
    bpSourceWaveBox.onChange = [this] {
        audioProcessor.setBpSourceWave();
        enableSourceGui(bpSourceWaveBox.getSelectedId());
    };
    bpSourceWaveBox.setSelectedItemIndex(0);

    // Bandpass source pulsewidth (square)
    bpSourcePwSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_PW_ID, bpSourcePwSlider);
    addAndMakeVisible(&bpSourcePwSlider);
    bpSourcePwSlider.setRange(0.0, 1.0);
    bpSourcePwSlider.setValue(0.5);
    bpSourcePwSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourcePwSlider.setLookAndFeel(&sourceLookAndFeel);
    bpSourcePwSlider.onValueChange = [this] {
        audioProcessor.setBpSourcePw();
    };
    addAndMakeVisible(&bpSourcePwLabel);
    bpSourcePwLabel.setText("Pulsewidth", juce::dontSendNotification);
    //bpSourcePwLabel.attachToComponent(&bpSourcePwSlider, true);
    bpSourcePwLabel.setLookAndFeel(&textLookAndFeel);

    // Bandpass source pressure (glottal model)
    bpSourcePressureAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_PRESSURE_ID, bpSourcePressureSlider);
    addAndMakeVisible(&bpSourcePressureSlider);
    bpSourcePressureSlider.setRange(0.0, 1.0);
    bpSourcePressureSlider.setValue(1.0);
    bpSourcePressureSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourcePressureSlider.setLookAndFeel(&sourceLookAndFeel);
    bpSourcePressureSlider.onValueChange = [this] {
        audioProcessor.setBpSourcePressure();
    };
    addAndMakeVisible(&bpSourcePressureLabel);
    bpSourcePressureLabel.setText("Pressure", juce::dontSendNotification);
    //bpSourcePressureLabel.attachToComponent(&bpSourcePressureSlider, false);
    bpSourcePressureLabel.setLookAndFeel(&textLookAndFeel);

    // Bandpass source T0 (glottal model)
    bpSourceT0Attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_T0_ID, bpSourceT0Slider);
    addAndMakeVisible(&bpSourceT0Slider);
    bpSourceT0Slider.setRange(0.0, 1.0);
    bpSourceT0Slider.setValue(0.944);
    bpSourceT0Slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceT0Slider.setLookAndFeel(&sourceLookAndFeel);
    bpSourceT0Slider.onValueChange = [this] {
        audioProcessor.setBpSourceT0();
    };
    addAndMakeVisible(&bpSourceT0Label);
    bpSourceT0Label.setText("T0", juce::dontSendNotification);
    //bpSourceT0Label.attachToComponent(&bpSourceT0Slider, false);
    bpSourceT0Label.setLookAndFeel(&textLookAndFeel);

    // Bandpass source Te (glottal model)
    bpSourceTeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_TE_ID, bpSourceTeSlider);
    addAndMakeVisible(&bpSourceTeSlider);
    bpSourceTeSlider.setRange(0.0, 1.0);
    bpSourceTeSlider.setValue(0.224);
    bpSourceTeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceTeSlider.setLookAndFeel(&sourceLookAndFeel);
    bpSourceTeSlider.onValueChange = [this] {
        audioProcessor.setBpSourceTe();
    };
    addAndMakeVisible(&bpSourceTeLabel);
    bpSourceTeLabel.setText("Te", juce::dontSendNotification);
    //bpSourceTeLabel.attachToComponent(&bpSourceTeSlider, false);
    bpSourceTeLabel.setLookAndFeel(&textLookAndFeel);

    // Bandpass source noise (glottal model)
    bpSourceNoiseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, SOURCE_NOISE_ID, bpSourceNoiseSlider);
    addAndMakeVisible(&bpSourceNoiseSlider);
    bpSourceNoiseSlider.setRange(0.0, 1.0);
    bpSourceNoiseSlider.setValue(0.0);
    bpSourceNoiseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpSourceNoiseSlider.setLookAndFeel(&sourceLookAndFeel);
    bpSourceNoiseSlider.onValueChange = [this] {
        audioProcessor.setBpSourceNoise();
    };
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
        audioProcessor.setFricativeColour();
    };

    // Fricative low cut slider
    fricativeLowCutAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_LOWCUT_ID, fricativeLowCutSlider);
    addAndMakeVisible(&fricativeLowCutSlider);
    fricativeLowCutSlider.setRange(audioProcessor.apvts.getParameterRange(FRICA_LOWCUT_ID).start, audioProcessor.apvts.getParameterRange(FRICA_LOWCUT_ID).end);
    fricativeLowCutSlider.setValue(audioProcessor.apvts.getParameterRange(FRICA_LOWCUT_ID).start);
    fricativeLowCutSlider.onValueChange = [this] {
        if (fricativeHighCutSlider.getValue() < fricativeLowCutSlider.getValue()) {
            fricativeHighCutSlider.setValue(fricativeLowCutSlider.getValue());
        }
        audioProcessor.setFricativeLowCut();
    };

    // Fricative high cut slider
    fricativeHighCutAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_HIGHCUT_ID, fricativeHighCutSlider);
    addAndMakeVisible(&fricativeHighCutSlider);
    fricativeHighCutSlider.setRange(audioProcessor.apvts.getParameterRange(FRICA_HIGHCUT_ID).start, audioProcessor.apvts.getParameterRange(FRICA_HIGHCUT_ID).end);
    fricativeHighCutSlider.setValue(audioProcessor.apvts.getParameterRange(FRICA_HIGHCUT_ID).end);
    fricativeHighCutSlider.onValueChange = [this] {
        if (fricativeHighCutSlider.getValue() < fricativeLowCutSlider.getValue()) {
            fricativeLowCutSlider.setValue(fricativeHighCutSlider.getValue());
        }
        audioProcessor.setFricativeHighCut();
    };

    /*Filter*/
    // Phoneme slider
    phonemeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, PHONEME_ID, phonemeSlider);
    addAndMakeVisible(&phonemeSlider);
    phonemeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    phonemeSlider.setRange(audioProcessor.apvts.getParameterRange(PHONEME_ID).start, audioProcessor.apvts.getParameterRange(PHONEME_ID).end);
    phonemeSlider.onValueChange = [this] {
        audioProcessor.setPhoneme(audioProcessor.phonemeVector, *audioProcessor.apvts.getRawParameterValue(PHONEME_ID));
        updateFilterControls(audioProcessor.interpolatedPhoneme);
        phonemeLabel.setText(audioProcessor.interpolatedPhoneme.getName(), juce::dontSendNotification);
    };
    addAndMakeVisible(&phonemeLabel);
    phonemeLabel.setText("Phoneme", juce::dontSendNotification);
    phonemeLabel.attachToComponent(&phonemeSlider, false);

    // Formant shift slider
    formantShiftAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FORMANT_SHIFT_ID, formantShiftSlider);
    addAndMakeVisible(&formantShiftSlider);
    formantShiftSlider.setRange(audioProcessor.apvts.getParameterRange(FORMANT_SHIFT_ID).start, audioProcessor.apvts.getParameterRange(FORMANT_SHIFT_ID).end);
    formantShiftSlider.setValue(0.0f);
    formantShiftSlider.onValueChange = [this] {
        audioProcessor.setPhoneme(audioProcessor.phonemeVector, *audioProcessor.apvts.getRawParameterValue(PHONEME_ID));
        updateFilterControls(audioProcessor.interpolatedPhoneme);
    };
    addAndMakeVisible(&formantShiftLabel);
    formantShiftLabel.setText("Formant Shift", juce::dontSendNotification);
    formantShiftLabel.attachToComponent(&formantShiftSlider, false);

    // Add phoneme button
    addAndMakeVisible(&addPhonemeButton);
    addPhonemeButton.setButtonText("Add Phoneme");
    addPhonemeButton.onClick = [this] {
        audioProcessor.addPhonemeToVector(audioProcessor.interpolatedPhoneme);
    };

    // Load XML button
    addAndMakeVisible(&loadXmlButton);
    loadXmlButton.setButtonText("Load XML File");
    loadXmlButton.onClick = [this] {
        audioProcessor.loadButtonClicked();
    };

    // F1 frequency
    f1FreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F1_FREQ_ID, f1FreqSlider);
    addAndMakeVisible(&f1FreqSlider);
    f1FreqSlider.setSliderStyle(juce::Slider::LinearBar);
    f1FreqSlider.setLookAndFeel(&filterLookAndFeel);
    f1FreqSlider.setRange(100.0, 5000.0);
    f1FreqSlider.setValue(800.0);
    f1FreqSlider.onValueChange = [this] {
        audioProcessor.setF1Freq();
    };
    // F1 bandwidth
    f1BandwidthAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F1_BW_ID, f1BandwidthSlider);
    addAndMakeVisible(&f1BandwidthSlider);
    f1BandwidthSlider.setSliderStyle(juce::Slider::LinearBar);
    f1BandwidthSlider.setLookAndFeel(&filterLookAndFeel);
    f1BandwidthSlider.setRange(0.0, 1000.0);
    f1BandwidthSlider.setValue(80.0);
    f1BandwidthSlider.onValueChange = [this] {
        audioProcessor.setF1Bandwidth();
    };
    // F1 gain
    f1GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F1_GAIN_ID, f1GainSlider);
    addAndMakeVisible(&f1GainSlider);
    f1GainSlider.setSliderStyle(juce::Slider::LinearBar);
    f1GainSlider.setLookAndFeel(&filterLookAndFeel);
    f1GainSlider.setRange(0.0, 1.0);
    f1GainSlider.setValue(1.0);
    f1GainSlider.onValueChange = [this] {
        audioProcessor.setF1Gain();
    };
    // F2 frequency
    f2FreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F2_FREQ_ID, f2FreqSlider);
    addAndMakeVisible(&f2FreqSlider);
    f2FreqSlider.setSliderStyle(juce::Slider::LinearBar);
    f2FreqSlider.setLookAndFeel(&filterLookAndFeel);
    f2FreqSlider.setRange(100.0, 5000.0);
    f2FreqSlider.setValue(800.0);
    f2FreqSlider.onValueChange = [this] {
        audioProcessor.setF2Freq();
    };
    // F2 bandwidth
    f2BandwidthAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F2_BW_ID, f2BandwidthSlider);
    addAndMakeVisible(&f2BandwidthSlider);
    f2BandwidthSlider.setSliderStyle(juce::Slider::LinearBar);
    f2BandwidthSlider.setLookAndFeel(&filterLookAndFeel);
    f2BandwidthSlider.setRange(0.0, 1000.0);
    f2BandwidthSlider.setValue(80.0);
    f2BandwidthSlider.onValueChange = [this] {
        audioProcessor.setF2Bandwidth();
    };
    // F2 gain
    f2GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F2_GAIN_ID, f2GainSlider);
    addAndMakeVisible(&f2GainSlider);
    f2GainSlider.setSliderStyle(juce::Slider::LinearBar);
    f2GainSlider.setLookAndFeel(&filterLookAndFeel);
    f2GainSlider.setRange(0.0, 1.0);
    f2GainSlider.setValue(1.0);
    f2GainSlider.onValueChange = [this] {
        audioProcessor.setF2Gain();
    };
    // F3 frequency
    f3FreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F3_FREQ_ID, f3FreqSlider);
    addAndMakeVisible(&f3FreqSlider);
    f3FreqSlider.setSliderStyle(juce::Slider::LinearBar);
    f3FreqSlider.setLookAndFeel(&filterLookAndFeel);
    f3FreqSlider.setRange(100.0, 5000.0);
    f3FreqSlider.setValue(800.0);
    f3FreqSlider.onValueChange = [this] {
        audioProcessor.setF3Freq();
    };
    // F3 bandwidth
    f3BandwidthAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F3_BW_ID, f3BandwidthSlider);
    addAndMakeVisible(&f3BandwidthSlider);
    f3BandwidthSlider.setSliderStyle(juce::Slider::LinearBar);
    f3BandwidthSlider.setLookAndFeel(&filterLookAndFeel);
    f3BandwidthSlider.setRange(0.0, 1000.0);
    f3BandwidthSlider.setValue(80.0);
    f3BandwidthSlider.onValueChange = [this] {
        audioProcessor.setF3Bandwidth();
    };
    // F3 gain
    f3GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F3_GAIN_ID, f3GainSlider);
    addAndMakeVisible(&f3GainSlider);
    f3GainSlider.setSliderStyle(juce::Slider::LinearBar);
    f3GainSlider.setLookAndFeel(&filterLookAndFeel);
    f3GainSlider.setRange(0.0, 1.0);
    f3GainSlider.setValue(1.0);
    f3GainSlider.setNumDecimalPlacesToDisplay(0);
    f3GainSlider.onValueChange = [this] {
        audioProcessor.setF3Gain();
    };
    // F4 frequency
    f4FreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F4_FREQ_ID, f4FreqSlider);
    addAndMakeVisible(&f4FreqSlider);
    f4FreqSlider.setSliderStyle(juce::Slider::LinearBar);
    f4FreqSlider.setLookAndFeel(&filterLookAndFeel);
    f4FreqSlider.setRange(100.0, 5000.0);
    f4FreqSlider.setValue(800.0);
    f4FreqSlider.onValueChange = [this] {
        audioProcessor.setF4Freq();
    };
    // F4 bandwidth
    f4BandwidthAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F4_BW_ID, f4BandwidthSlider);
    addAndMakeVisible(&f4BandwidthSlider);
    f4BandwidthSlider.setSliderStyle(juce::Slider::LinearBar);
    f4BandwidthSlider.setLookAndFeel(&filterLookAndFeel);
    f4BandwidthSlider.setRange(0.0, 1000.0);
    f4BandwidthSlider.setValue(80.0);
    f4BandwidthSlider.onValueChange = [this] {
        audioProcessor.setF4Bandwidth();
    };
    // F4 gain
    f4GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F4_GAIN_ID, f4GainSlider);
    addAndMakeVisible(&f4GainSlider);
    f4GainSlider.setSliderStyle(juce::Slider::LinearBar);
    f4GainSlider.setLookAndFeel(&filterLookAndFeel);
    f4GainSlider.setRange(0.0, 1.0);
    f4GainSlider.setValue(1.0);
    f4GainSlider.onValueChange = [this] {
        audioProcessor.setF4Gain();
    };
    // F5 frequency
    f5FreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F5_FREQ_ID, f5FreqSlider);
    addAndMakeVisible(&f5FreqSlider);
    f5FreqSlider.setSliderStyle(juce::Slider::LinearBar);
    f5FreqSlider.setLookAndFeel(&filterLookAndFeel);
    f5FreqSlider.setRange(100.0, 5000.0);
    f5FreqSlider.setValue(800.0);
    f5FreqSlider.onValueChange = [this] {
        audioProcessor.setF5Freq();
    };
    // F5 bandwidth
    f5BandwidthAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F5_BW_ID, f5BandwidthSlider);
    addAndMakeVisible(&f5BandwidthSlider);
    f5BandwidthSlider.setSliderStyle(juce::Slider::LinearBar);
    f5BandwidthSlider.setLookAndFeel(&filterLookAndFeel);
    f5BandwidthSlider.setRange(0.0, 1000.0);
    f5BandwidthSlider.setValue(80.0);
    f5BandwidthSlider.onValueChange = [this] {
        audioProcessor.setF5Bandwidth();
    };
    // F5 gain
    f5GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, F5_GAIN_ID, f5GainSlider);
    addAndMakeVisible(&f5GainSlider);
    f5GainSlider.setSliderStyle(juce::Slider::LinearBar);
    f5GainSlider.setLookAndFeel(&filterLookAndFeel);
    f5GainSlider.setRange(0.0, 1.0);
    f5GainSlider.setValue(1.0);
    f5GainSlider.onValueChange = [this] {
        audioProcessor.setF5Gain();
    };
    // Voice envelope label
    addAndMakeVisible(&voiceEnvelopeLabel);
    voiceEnvelopeLabel.setText("Voice", juce::dontSendNotification);
    voiceEnvelopeLabel.setLookAndFeel(&textLookAndFeel);

    // Voice attack
    voiceAttachAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VOICE_ATTACK_ID, voiceAttackSlider);
    addAndMakeVisible(&voiceAttackSlider);
    voiceAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceAttackSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceAttackSlider.setRange(0.0, 2.0);
    voiceAttackSlider.setValue(0.05);
    voiceAttackSlider.onValueChange = [this] {
        audioProcessor.setVoiceAttack();
    };
    addAndMakeVisible(&voiceAttackLabel);
    voiceAttackLabel.setText("A", juce::dontSendNotification);
    voiceAttackLabel.attachToComponent(&voiceAttackSlider, false);
    voiceAttackLabel.setLookAndFeel(&textLookAndFeel);

    // Voice decay
    voiceDecayAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VOICE_DECAY_ID, voiceDecaySlider);
    addAndMakeVisible(&voiceDecaySlider);
    voiceDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceDecaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceDecaySlider.setLookAndFeel(&sourceLookAndFeel);
    voiceDecaySlider.setRange(0.0, 2.0);
    voiceDecaySlider.setValue(0.05);
    voiceDecaySlider.onValueChange = [this] {
        audioProcessor.setVoiceDecay();
    };
    addAndMakeVisible(&voiceDecayLabel);
    voiceDecayLabel.setText("D", juce::dontSendNotification);
    voiceDecayLabel.attachToComponent(&voiceDecaySlider, false);
    voiceDecayLabel.setLookAndFeel(&textLookAndFeel);

    // Voice sustain
    voiceSustainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VOICE_SUSTAIN_ID, voiceSustainSlider);
    addAndMakeVisible(&voiceSustainSlider);
    voiceSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceSustainSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceSustainSlider.setRange(0.0, 1.0);
    voiceSustainSlider.setValue(1.0);
    voiceSustainSlider.onValueChange = [this] {
        audioProcessor.setVoiceSustain();
    };
    addAndMakeVisible(&voiceSustainLabel);
    voiceSustainLabel.setText("S", juce::dontSendNotification);
    voiceSustainLabel.attachToComponent(&voiceSustainSlider, false);
    voiceSustainLabel.setLookAndFeel(&textLookAndFeel);

    // Voice release
    voiceReleaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VOICE_RELEASE_ID, voiceReleaseSlider);
    addAndMakeVisible(&voiceReleaseSlider);
    voiceReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    voiceReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    voiceReleaseSlider.setLookAndFeel(&sourceLookAndFeel);
    voiceReleaseSlider.setRange(0.0, 4.0);
    voiceReleaseSlider.setValue(1.0);
    voiceReleaseSlider.onValueChange = [this] {
        audioProcessor.setVoiceRelease();
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
    fricativeAttackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_ATTACK_ID, fricativeAttackSlider);
    addAndMakeVisible(&fricativeAttackSlider);
    fricativeAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeAttackSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeAttackSlider.setRange(0.0, 2.0);
    fricativeAttackSlider.setValue(0.01);
    fricativeAttackSlider.onValueChange = [this] {
        audioProcessor.setFricativeAttack();
    };
    addAndMakeVisible(&fricativeAttackLabel);
    fricativeAttackLabel.setText("A", juce::dontSendNotification);
    fricativeAttackLabel.attachToComponent(&fricativeAttackSlider, false);
    fricativeAttackLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative decay
    fricativeDecayAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_DECAY_ID, fricativeDecaySlider);
    addAndMakeVisible(&fricativeDecaySlider);
    fricativeDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeDecaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeDecaySlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeDecaySlider.setRange(0.0, 2.0);
    fricativeDecaySlider.setValue(0.05);
    fricativeDecaySlider.onValueChange = [this] {
        audioProcessor.setFricativeDecay();
    };
    addAndMakeVisible(&fricativeDecayLabel);
    fricativeDecayLabel.setText("D", juce::dontSendNotification);
    fricativeDecayLabel.attachToComponent(&fricativeDecaySlider, false);
    fricativeDecayLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative sustain
    fricativeSustainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_SUSTAIN_ID, fricativeSustainSlider);
    addAndMakeVisible(&fricativeSustainSlider);
    fricativeSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeSustainSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeSustainSlider.setRange(0.0, 1.0);
    fricativeSustainSlider.setValue(0.0);
    fricativeSustainSlider.onValueChange = [this] {
        audioProcessor.setFricativeSustain();
    };
    addAndMakeVisible(&fricativeSustainLabel);
    fricativeSustainLabel.setText("S", juce::dontSendNotification);
    fricativeSustainLabel.attachToComponent(&fricativeSustainSlider, false);
    fricativeSustainLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative release
    fricativeReleaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_RELEASE_ID, fricativeReleaseSlider);
    addAndMakeVisible(&fricativeReleaseSlider);
    fricativeReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    fricativeReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeReleaseSlider.setLookAndFeel(&fricativeLookAndFeel);
    fricativeReleaseSlider.setRange(0.0, 4.0);
    fricativeReleaseSlider.setValue(1.0);
    fricativeReleaseSlider.onValueChange = [this] {
        audioProcessor.setFricativeRelease();
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
    vibratoFrequencyAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VIBRATO_FREQUENCY_ID, vibratoFrequencySlider);
    addAndMakeVisible(&vibratoFrequencySlider);
    vibratoFrequencySlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoFrequencySlider.setRange(0.0, 8.0);
    vibratoFrequencySlider.setValue(5.0);
    vibratoFrequencySlider.onValueChange = [this] {
        audioProcessor.setVibratoFrequency();
    };
    addAndMakeVisible(&vibratoFrequencyLabel);
    vibratoFrequencyLabel.setText("Freq", juce::dontSendNotification);
    vibratoFrequencyLabel.attachToComponent(&vibratoFrequencySlider, false);
    vibratoFrequencyLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato attack
    vibratoAttackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VIBRATO_ATTACK_ID, vibratoAttackSlider);
    addAndMakeVisible(&vibratoAttackSlider);
    vibratoAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoAttackSlider.setRange(0.0, 6.0);
    vibratoAttackSlider.setValue(1.0);
    vibratoAttackSlider.onValueChange = [this] {
        audioProcessor.setVibratoAttack();
    };
    addAndMakeVisible(&vibratoAttackLabel);
    vibratoAttackLabel.setText("A", juce::dontSendNotification);
    vibratoAttackLabel.attachToComponent(&vibratoAttackSlider, false);
    vibratoAttackLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato sustain
    vibratoSustainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VIBRATO_SUSTAIN_ID, vibratoSustainSlider);
    addAndMakeVisible(&vibratoSustainSlider);
    vibratoSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoSustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoSustainSlider.setRange(0.0, 1.0);
    vibratoSustainSlider.setValue(1.0);
    vibratoSustainSlider.onValueChange = [this] {
        audioProcessor.setVibratoSustain();
    };
    addAndMakeVisible(&vibratoSustainLabel);
    vibratoSustainLabel.setText("S", juce::dontSendNotification);
    vibratoSustainLabel.attachToComponent(&vibratoSustainSlider, false);
    vibratoSustainLabel.setLookAndFeel(&textLookAndFeel);

    // Vibrato release
    vibratoReleaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, VIBRATO_RELEASE_ID, vibratoReleaseSlider);
    addAndMakeVisible(&vibratoReleaseSlider);
    vibratoReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    vibratoReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    vibratoReleaseSlider.setRange(0.0, 6.0);
    vibratoReleaseSlider.setValue(1.0);
    vibratoReleaseSlider.onValueChange = [this] {
        audioProcessor.setVibratoRelease();
    };
    addAndMakeVisible(&vibratoReleaseLabel);
    vibratoReleaseLabel.setText("R", juce::dontSendNotification);
    vibratoReleaseLabel.attachToComponent(&vibratoReleaseSlider, false);
    vibratoReleaseLabel.setLookAndFeel(&textLookAndFeel);

    // FOF Gain
    fofGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FOF_GAIN_ID, fofGainSlider);
    addAndMakeVisible(&fofGainSlider);
    fofGainSlider.setRange(0.0, 1.0);
    fofGainSlider.setValue(1.0);
    fofGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fofGainSlider.setLookAndFeel(&mixerLookAndFeel);
    fofGainSlider.onValueChange = [this] {
        audioProcessor.setFofGain();
    };
    addAndMakeVisible(&fofGainLabel);
    fofGainLabel.setText("FOF Gain", juce::dontSendNotification);
    fofGainLabel.attachToComponent(&fofGainSlider, false);
    fofGainLabel.setLookAndFeel(&textLookAndFeel);

    // FOF gain lock
    fofGainLockAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, FOF_LOCK_ID, fofGainLockButton);
    addAndMakeVisible(&fofGainLockButton);

    // BP Gain
    bpGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, BP_GAIN_ID, bpGainSlider);
    addAndMakeVisible(&bpGainSlider);
    bpGainSlider.setRange(0.0, 1.0);
    bpGainSlider.setValue(1.0);
    bpGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    bpGainSlider.setLookAndFeel(&mixerLookAndFeel);
    bpGainSlider.onValueChange = [this] {
        audioProcessor.setBpGain();
    };
    addAndMakeVisible(&bpGainLabel);
    bpGainLabel.setText("BP Gain", juce::dontSendNotification);
    bpGainLabel.attachToComponent(&bpGainSlider, false);
    bpGainLabel.setLookAndFeel(&textLookAndFeel);

    // BP gain lock
    bpGainLockAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, BP_LOCK_ID, bpGainLockButton);
    addAndMakeVisible(&bpGainLockButton);

    // Fricative Gain
    fricativeGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, FRICA_GAIN_ID, fricativeGainSlider);
    addAndMakeVisible(&fricativeGainSlider);
    fricativeGainSlider.setRange(0.0, 1.0);
    fricativeGainSlider.setValue(1.0);
    fricativeGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    fricativeGainSlider.setLookAndFeel(&mixerLookAndFeel);
    fricativeGainSlider.onValueChange = [this] {
        audioProcessor.setFricativeGain();
    };
    addAndMakeVisible(&fricativeGainLabel);
    fricativeGainLabel.setText("Fricative Gain", juce::dontSendNotification);
    fricativeGainLabel.attachToComponent(&fricativeGainSlider, false);
    fricativeGainLabel.setLookAndFeel(&textLookAndFeel);

    // Fricative gain lock
    fricativeGainLockAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, FRICA_LOCK_ID, fricativeGainLockButton);
    addAndMakeVisible(&fricativeGainLockButton);


    // Midi message table
    addAndMakeVisible(&table);
}

FormantSynthAudioProcessorEditor::~FormantSynthAudioProcessorEditor()
{
    audioProcessor.keyboardState.removeListener(this);
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
        juce::FlexItem(fricativeLowCutSlider).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight),
        juce::FlexItem(fricativeHighCutSlider).withFlex(1, 1, sourceArea.getHeight() / 16).withMinWidth(50.0).withMinHeight(30.0).withWidth(sourceArea.getWidth() - objectBorder).withHeight(objectHeight) });

    sourceFB.performLayout(sourceArea.reduced(objectBorder).toFloat());

    

    keyboardComponent.setBounds(keyboardArea);
    headerWindow.setBounds(headerArea.reduced(windowBorder));
    sourceWindow.setBounds(sourceArea.reduced(windowBorder));
    filterWindow.setBounds(filterArea.reduced(windowBorder));
    vibratoWindow.setBounds(vibratoArea.reduced(windowBorder));
    envelopeWindow.setBounds(envelopeArea.reduced(windowBorder));
    mixerWindow.setBounds(mixerArea.reduced(windowBorder));

    auto envelopeHeaderArea = envelopeArea.removeFromTop(headerHeight);

    phonemeSlider.setBounds(filterArea.removeFromBottom(100).reduced(objectBorder));
    formantShiftSlider.setBounds(filterArea.removeFromBottom(100).reduced(objectBorder));
    addPhonemeButton.setBounds(filterArea.removeFromTop(50).removeFromLeft(filterArea.getWidth()/2).reduced(objectBorder));
    loadXmlButton.setBounds(filterArea);

    /*auto f1Area = formantArea.removeFromLeft(formantArea.getWidth() / 5);
    f1FreqSlider.setBounds(f1Area.removeFromTop(f1Area.getHeight() / 3).reduced(0));
    f1BandwidthSlider.setBounds(f1Area.removeFromTop(f1Area.getHeight() / 2).reduced(0));
    f1GainSlider.setBounds(f1Area.reduced(0));
    auto f2Area = formantArea.removeFromLeft(formantArea.getWidth() / 4);
    f2FreqSlider.setBounds(f2Area.removeFromTop(f2Area.getHeight() / 3).reduced(0));
    f2BandwidthSlider.setBounds(f2Area.removeFromTop(f2Area.getHeight() / 2).reduced(0));
    f2GainSlider.setBounds(f2Area.reduced(0));
    auto f3Area = formantArea.removeFromLeft(formantArea.getWidth() / 3);
    f3FreqSlider.setBounds(f3Area.removeFromTop(f3Area.getHeight() / 3).reduced(0));
    f3BandwidthSlider.setBounds(f3Area.removeFromTop(f3Area.getHeight() / 2).reduced(0));
    f3GainSlider.setBounds(f3Area.reduced(0));
    auto f4Area = formantArea.removeFromLeft(formantArea.getWidth() / 2);
    f4FreqSlider.setBounds(f4Area.removeFromTop(f4Area.getHeight() / 3).reduced(0));
    f4BandwidthSlider.setBounds(f4Area.removeFromTop(f4Area.getHeight() / 2).reduced(0));
    f4GainSlider.setBounds(f4Area.reduced(0));
    auto f5Area = formantArea.removeFromLeft(formantArea.getWidth() / 1);
    f5FreqSlider.setBounds(f5Area.removeFromTop(f5Area.getHeight() / 3).reduced(0));
    f5BandwidthSlider.setBounds(f5Area.removeFromTop(f5Area.getHeight() / 2).reduced(0));
    f5GainSlider.setBounds(f5Area.reduced(0));*/

    table.setBounds(formantArea.reduced(objectBorder));

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

    auto mixerLockArea = mixerArea.removeFromRight(60);
    fofGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight()/3).reduced(objectBorder));
    bpGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight() / 2).reduced(objectBorder));
    fricativeGainSlider.setBounds(mixerArea.removeFromTop(mixerArea.getHeight()).reduced(objectBorder));
    fofGainLockButton.setBounds(mixerLockArea.removeFromTop(mixerLockArea.getHeight() / 3).reduced(objectBorder));
    bpGainLockButton.setBounds(mixerLockArea.removeFromTop(mixerLockArea.getHeight() / 2).reduced(objectBorder));
    fricativeGainLockButton.setBounds(mixerLockArea.removeFromTop(mixerLockArea.getHeight()).reduced(objectBorder));

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


void FormantSynthAudioProcessorEditor::updateFilterControls(Phoneme p)
{
    //fofGainSlider.setValue(p.getFofGain());
    //bpGainSlider.setValue(p.getBpGain());

    f1FreqSlider.setValue(p.getFrequency(0));
    f1BandwidthSlider.setValue(p.getBandwidth(0));
    f1GainSlider.setValue(p.getGain(0));

    f2FreqSlider.setValue(p.getFrequency(1));
    f2BandwidthSlider.setValue(p.getBandwidth(1));
    f2GainSlider.setValue(p.getGain(1));

    f3FreqSlider.setValue(p.getFrequency(2));
    f3BandwidthSlider.setValue(p.getBandwidth(2));
    f3GainSlider.setValue(p.getGain(2));

    f4FreqSlider.setValue(p.getFrequency(3));
    f4BandwidthSlider.setValue(p.getBandwidth(3));
    f4GainSlider.setValue(p.getGain(3));

    f5FreqSlider.setValue(p.getFrequency(4));
    f5BandwidthSlider.setValue(p.getBandwidth(4));
    f5GainSlider.setValue(p.getGain(4));

    fricativeLowCutSlider.setValue(p.getFricativeLow());
    fricativeHighCutSlider.setValue(p.getFricativeHigh());
    fricativeAttackSlider.setValue(p.getFricativeAttack());
    fricativeDecaySlider.setValue(p.getFricativeDecay());
    fricativeSustainSlider.setValue(p.getFricativeSustain());
    fricativeReleaseSlider.setValue(p.getFricativeRelease());

    if (*audioProcessor.apvts.getRawParameterValue(FRICA_LOCK_ID) != 1) {
        fricativeGainSlider.setValue(p.getFricativeGain());
    }
    if (*audioProcessor.apvts.getRawParameterValue(FOF_LOCK_ID) != 1) {
        fofGainSlider.setValue(p.getFofGain());
    }
    if (*audioProcessor.apvts.getRawParameterValue(BP_LOCK_ID) != 1) {
        bpGainSlider.setValue(p.getBpGain());
    }
}

