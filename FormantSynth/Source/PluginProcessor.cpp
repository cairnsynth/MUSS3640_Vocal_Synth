#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FormantSynthAudioProcessor::FormantSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
    apvts(*this, nullptr, "PARAMETERS", createParameterLayout())  // Initialise value tree
#endif
{
    initialisePhonemes();  // Load default phoneme values
    startTimerHz(60);  // Start Midi input timer at 60Hz frequency
    dsp.start();  // Start Faust DSP code
}

FormantSynthAudioProcessor::~FormantSynthAudioProcessor()
{
    stopTimer();  // Stop timer when plugin exits
    dsp.stop();  // Stop DSP when plugin exits
}

//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout FormantSynthAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;  // Temporary vector of parameters to load into value tree
    // Source parameter creation
    auto sourceWaveParam = std::make_unique<juce::AudioParameterInt>(SOURCE_WAVE_ID, "Source Waveform", 0, 2, 0);
    params.push_back(std::move(sourceWaveParam));
    auto sourcePwParam = std::make_unique<juce::AudioParameterFloat>(SOURCE_PW_ID, "Source Wave Pulsewidth", 0.0f, 1.0f, 0.25f);
    params.push_back(std::move(sourcePwParam));
    auto sourcePressureParam = std::make_unique<juce::AudioParameterFloat>(SOURCE_PRESSURE_ID, "Source Pressure", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(sourcePressureParam));
    auto sourceT0Param = std::make_unique<juce::AudioParameterFloat>(SOURCE_T0_ID, "Source T0 Value", 0.0f, 1.0f, 0.944f);
    params.push_back(std::move(sourceT0Param));
    auto sourceTeParam = std::make_unique<juce::AudioParameterFloat>(SOURCE_TE_ID, "Source Te Value", 0.0f, 1.0f, 0.223f);
    params.push_back(std::move(sourceTeParam));
    auto sourceNoiseParam = std::make_unique<juce::AudioParameterFloat>(SOURCE_NOISE_ID, "Source Noise Gain", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(sourceNoiseParam));
    auto monoParam = std::make_unique<juce::AudioParameterBool>(MONO_ID, "Mono/Poly", true);
    params.push_back(std::move(monoParam));
    auto fricativeLowCutParam = std::make_unique<juce::AudioParameterFloat>(FRICA_LOWCUT_ID, "Fricative Low Cut", 20.0f, 20000.0f, 20.0f);
    params.push_back(std::move(fricativeLowCutParam));
    auto fricativeHighCutParam = std::make_unique<juce::AudioParameterFloat>(FRICA_HIGHCUT_ID, "Fricative Low Cut", 20.0f, 20000.0f, 20000.0f);
    params.push_back(std::move(fricativeHighCutParam));
    // Filter parameter creation
    auto phonemeParam = std::make_unique<juce::AudioParameterFloat>(PHONEME_ID, "Phoneme", 0.0f, 4.0f, 0.0f);
    params.push_back(std::move(phonemeParam));
    auto formantShiftParam = std::make_unique<juce::AudioParameterFloat>(FORMANT_SHIFT_ID, "Formant Shift", -500.0f, 500.0f, 0.0f);
    params.push_back(std::move(formantShiftParam));
    auto skirtWidthParam = std::make_unique<juce::AudioParameterBool>(SKIRTWIDTH_ID, "Use Skirt Width", 1);
    params.push_back(std::move(skirtWidthParam));
    // Formant parameter creation
    auto f1FreqParam = std::make_unique<juce::AudioParameterFloat>(F1_FREQ_ID, "Formant 1 Frequency", 20.0f, 20000.0f, 800.0f);
    params.push_back(std::move(f1FreqParam));
    auto f1BandwidthParam = std::make_unique<juce::AudioParameterFloat>(F1_BW_ID, "Formant 1 Bandwidth", 20.0f, 10000.0f, 80.0f);
    params.push_back(std::move(f1BandwidthParam));
    auto f1GainParam = std::make_unique<juce::AudioParameterFloat>(F1_GAIN_ID, "Formant 1 Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(f1GainParam));

    auto f2FreqParam = std::make_unique<juce::AudioParameterFloat>(F2_FREQ_ID, "Formant 2 Frequency", 20.0f, 20000.0f, 800.0f);
    params.push_back(std::move(f2FreqParam));
    auto f2BandwidthParam = std::make_unique<juce::AudioParameterFloat>(F2_BW_ID, "Formant 2 Bandwidth", 20.0f, 10000.0f, 80.0f);
    params.push_back(std::move(f2BandwidthParam));
    auto f2GainParam = std::make_unique<juce::AudioParameterFloat>(F2_GAIN_ID, "Formant 2 Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(f2GainParam));

    auto f3FreqParam = std::make_unique<juce::AudioParameterFloat>(F3_FREQ_ID, "Formant 3 Frequency", 20.0f, 20000.0f, 800.0f);
    params.push_back(std::move(f3FreqParam));
    auto f3BandwidthParam = std::make_unique<juce::AudioParameterFloat>(F3_BW_ID, "Formant 3 Bandwidth", 20.0f, 10000.0f, 80.0f);
    params.push_back(std::move(f3BandwidthParam));
    auto f3GainParam = std::make_unique<juce::AudioParameterFloat>(F3_GAIN_ID, "Formant 3 Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(f3GainParam));

    auto f4FreqParam = std::make_unique<juce::AudioParameterFloat>(F4_FREQ_ID, "Formant 4 Frequency", 20.0f, 20000.0f, 800.0f);
    params.push_back(std::move(f4FreqParam));
    auto f4BandwidthParam = std::make_unique<juce::AudioParameterFloat>(F4_BW_ID, "Formant 4 Bandwidth", 20.0f, 10000.0f, 80.0f);
    params.push_back(std::move(f4BandwidthParam));
    auto f4GainParam = std::make_unique<juce::AudioParameterFloat>(F4_GAIN_ID, "Formant 4 Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(f4GainParam));

    auto f5FreqParam = std::make_unique<juce::AudioParameterFloat>(F5_FREQ_ID, "Formant 5 Frequency", 20.0f, 20000.0f, 800.0f);
    params.push_back(std::move(f5FreqParam));
    auto f5BandwidthParam = std::make_unique<juce::AudioParameterFloat>(F5_BW_ID, "Formant 5 Bandwidth", 20.0f, 10000.0f, 80.0f);
    params.push_back(std::move(f5BandwidthParam));
    auto f5GainParam = std::make_unique<juce::AudioParameterFloat>(F5_GAIN_ID, "Formant 5 Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(f5GainParam));
    // Vibrato parameter creation
    auto vibratoFreqParam = std::make_unique<juce::AudioParameterFloat>(VIBRATO_FREQUENCY_ID, "Vibrato Frequency", 0.0f, 8.0f, 5.0f);
    params.push_back(std::move(vibratoFreqParam));
    auto vibratoAttackParam = std::make_unique<juce::AudioParameterFloat>(VIBRATO_ATTACK_ID, "Vibrato Attack", 0.0f, 4.0f, 0.5f);
    params.push_back(std::move(vibratoAttackParam));
    auto vibratoSustainParam = std::make_unique<juce::AudioParameterFloat>(VIBRATO_SUSTAIN_ID, "Vibrato Sustain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(vibratoSustainParam));
    auto vibratoReleaseParam = std::make_unique<juce::AudioParameterFloat>(VIBRATO_RELEASE_ID, "Vibrato Release", 0.0f, 5.0f, 1.0f);
    params.push_back(std::move(vibratoReleaseParam));
    // Envlope parameter creation
    auto voiceAttackParam = std::make_unique<juce::AudioParameterFloat>(VOICE_ATTACK_ID, "Voice Envelope Attack", 0.0f, 4.0f, 0.01f);
    params.push_back(std::move(voiceAttackParam));
    auto voiceDecayParam = std::make_unique<juce::AudioParameterFloat>(VOICE_DECAY_ID, "Voice Envelope Decay", 0.0f, 4.0f, 0.5f);
    params.push_back(std::move(voiceDecayParam));
    auto voiceSustainParam = std::make_unique<juce::AudioParameterFloat>(VOICE_SUSTAIN_ID, "Voice Envelope Sustain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(voiceSustainParam));
    auto voiceReleaseParam = std::make_unique<juce::AudioParameterFloat>(VOICE_RELEASE_ID, "Voice Envelope Release", 0.0f, 5.0f, 1.0f);
    params.push_back(std::move(voiceReleaseParam));

    auto fricativeAttackParam = std::make_unique<juce::AudioParameterFloat>(FRICA_ATTACK_ID, "Fricative Envelope Attack", 0.0f, 4.0f, 0.01f);
    params.push_back(std::move(fricativeAttackParam));
    auto fricativeDecayParam = std::make_unique<juce::AudioParameterFloat>(FRICA_DECAY_ID, "Fricative Envelope Decay", 0.0f, 4.0f, 0.5f);
    params.push_back(std::move(fricativeDecayParam));
    auto fricativeSustainParam = std::make_unique<juce::AudioParameterFloat>(FRICA_SUSTAIN_ID, "Fricative Envelope Sustain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(fricativeSustainParam));
    auto fricativeReleaseParam = std::make_unique<juce::AudioParameterFloat>(FRICA_RELEASE_ID, "Frivative Envelope Release", 0.0f, 5.0f, 1.0f);
    params.push_back(std::move(fricativeReleaseParam));
    // Mixer parameter creation
    auto fofGainParam = std::make_unique<juce::AudioParameterFloat>(FOF_GAIN_ID, "FOF Model Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(fofGainParam));
    auto fofGainLockParam = std::make_unique<juce::AudioParameterBool>(FOF_LOCK_ID, "FOF Gain Lock", false);
    params.push_back(std::move(fofGainLockParam));
    auto bpGainParam = std::make_unique<juce::AudioParameterFloat>(BP_GAIN_ID, "Bandpass Model Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(bpGainParam));
    auto bpGainLockParam = std::make_unique<juce::AudioParameterBool>(BP_LOCK_ID, "Bandpass Gain Lock", false);
    params.push_back(std::move(bpGainLockParam));
    auto fricativeGainParam = std::make_unique<juce::AudioParameterFloat>(FRICA_GAIN_ID, "Fricative Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(fricativeGainParam));
    auto fricativeGainLockParam = std::make_unique<juce::AudioParameterBool>(FRICA_LOCK_ID, "Fricative Gain Lock", false);
    params.push_back(std::move(fricativeGainLockParam));

    return { params.begin(), params.end() };  // Returns parameter vector
}

void FormantSynthAudioProcessor::initialisePhonemes()
{
    Phoneme defaultA;  // Temporary phoneme object to store defaut values
    // Setting intial formant values of the phoneme
    defaultA.setFormant(0, 600, 60, 1.0);
    defaultA.setFormant(1, 1040, 70, 0.199526);
    defaultA.setFormant(2, 2250, 110, 0.1259);
    defaultA.setFormant(3, 2450, 120, 0.1259);
    defaultA.setFormant(4, 2750, 130, 0.01);
    defaultA.setName("A");
    Phoneme defaultE;
    defaultE.setFormant(0, 400, 40, 1.0);
    defaultE.setFormant(1, 1620, 80, 0.063096);
    defaultE.setFormant(2, 2400, 100, 0.125893);
    defaultE.setFormant(3, 3800, 120, 0.063096);
    defaultE.setFormant(4, 3150, 120, 0.015849);
    defaultE.setName("E");
    Phoneme defaultI;
    defaultI.setFormant(0, 250, 60, 1.0);
    defaultI.setFormant(1, 1740, 90, 0.001);
    defaultI.setFormant(2, 2600, 100, 0.025119);
    defaultI.setFormant(3, 3050, 120, 0.00631);
    defaultI.setFormant(4, 3340, 120, 0.001585);
    defaultI.setName("I");
    Phoneme defaultO;
    defaultO.setFormant(0, 400, 40, 1.0);
    defaultO.setFormant(1, 750, 80, 0.079433);
    defaultO.setFormant(2, 2400, 100, 0.007943);
    defaultO.setFormant(3, 2600, 120, 0.01);
    defaultO.setFormant(4, 2900, 120, 0.0001);
    defaultO.setName("O");
    Phoneme defaultU;
    defaultU.setFormant(0, 350, 40, 1.0);
    defaultU.setFormant(1, 600, 80, 0.01);
    defaultU.setFormant(2, 2400, 100, 0.000631);
    defaultU.setFormant(3, 2675, 120, 0.001585);
    defaultU.setFormant(4, 2950, 120, 0.001585);
    defaultU.setName("U");
    // Add default phoneme objects to phoneme vector
    phonemeVector.push_back(std::move(defaultA));
    phonemeVector.push_back(std::move(defaultE));
    phonemeVector.push_back(std::move(defaultI));
    phonemeVector.push_back(std::move(defaultO));
    phonemeVector.push_back(std::move(defaultU));
}

const juce::String FormantSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FormantSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FormantSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FormantSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FormantSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FormantSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FormantSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FormantSynthAudioProcessor::setCurrentProgram(int index) {}

const juce::String FormantSynthAudioProcessor::getProgramName (int index) { return {}; }

void FormantSynthAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

//==============================================================================
void FormantSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {}

void FormantSynthAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FormantSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif
    return true;
  #endif
}
#endif

void FormantSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    midiQueue.push(midiMessages); // Push new MIDI messages onto queue
    buffer.clear();  // Clear output buffer, as DSP code handles audio output
    processMidi();  // Trigger notes based off incoming MIDI messages
}

void FormantSynthAudioProcessor::timerCallback()
{
    std::vector<juce::MidiMessage> messages;  // Vector to store messages
    midiQueue.pop(std::back_inserter(messages));  // Pop messages from queue into vector
    midiModel.addMessages(messages.begin(), messages.end());  // Insert messages into midiModel for processing
}

void FormantSynthAudioProcessor::processMidi()
{  
    for (juce::MidiMessage m : midiModel.newMessages) {  // Loop through MIDI messages stored in midiModel
        if (m.isNoteOn()) {  // If MIDI message is a 'Note On' message
            bool isOnFlag = 0;  // Note on flag
            for (auto i = voiceKeys.begin(); i != voiceKeys.end(); ++i) {  // Loop through active keys
                if (m.getNoteNumber() == *i) {  // If key is already on
                    isOnFlag = 1;  // Set flag
                }
            }
            if (!isOnFlag) {  // If key is not already on
                keyOn(m.getNoteNumber(), m.getVelocity());  // Create new DSP voice with pitch, velocity from MIDI message
                voiceKeys.push_back(m.getNoteNumber());  // Add key to list of active keys
            }
            isOnFlag = 0;  // Reset flag
        }
        if (m.isNoteOff()) {  // If MIDI message is a 'Note Off' message
            for (auto i = voiceKeys.begin(); i != voiceKeys.end(); ++i) {  // Loop through active keys
                if (m.getNoteNumber() == *i) {  // If MIDI message key matches active key
                    keyOff(m.getNoteNumber());  // Destroy DSP voice
                    voiceKeys.erase(i);  // Remove key from active keys
                }
            }
        }
        if (m.isAllNotesOff()) {  // If MIDI message is 'All Notes Off' message
            dsp.allNotesOff();  // Destroys all DSP voices
            voiceKeys.clear();  // Clear active voices
        }
    }
}

//==============================================================================
bool FormantSynthAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* FormantSynthAudioProcessor::createEditor()
{
    return new FormantSynthAudioProcessorEditor (*this);
}

//==============================================================================
/* getStateInformation / setStateInformation Reference :
*  https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
*/

/* --getStateInformation(juce::MemoryBlock& destData)-- 
* Handles saving values of parameters when instance of plugin is closed
* Called by DAW on saving a project
* 
* REFERENCE:
*   https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
*/
void FormantSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();  // Copy values from value tree into temp state
    std::unique_ptr<juce::XmlElement> stateXml(state.createXml());  // Create XML tree of state
    copyXmlToBinary(*stateXml, destData);  // save XML tree into destination as binary
}

/* --setStateInformation(const void* data, int sizeInBytes)--

* Handles loading of parameters into value tree when new instance of plugin is
* opened
* Called by DAW on opening a saved project
* 
* REFERENCE:
*   https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
*/
void FormantSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> stateXml(getXmlFromBinary(data, sizeInBytes));  // convert provided binary to XML tree
    if (stateXml.get() != nullptr) {  // If there is data
        if (stateXml->hasTagName(apvts.state.getType())) {  // If XML data matches type of value tree
            apvts.replaceState(juce::ValueTree::fromXml(*stateXml));  // Set values in value tree to values stored in data
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FormantSynthAudioProcessor();
}

//==============================================================================
//End JUCE generated Code

void FormantSynthAudioProcessor::keyOn(int key, int velocity)
{
    // If statement to switch between monophonic/polyphonic voice creation
    if (1) {  // If polyphonic. Always true; mono not fully implemented
        dsp.keyOn(key, velocity);  // Create new DSP voice with given key, velocity
    }
    else {  // If monophonic
        if (monoVoiceId == 0) {  // If mono voice does not yet exist
            monoVoiceId = dsp.keyOn(key, velocity);  // Create new DSP voice with given key, velocity
        }
        else {  // If mono voice exists
            // Update mono voice with new key, velocity, and trigger gate
            dsp.setVoiceParamValue("/FormantSynth/freq", monoVoiceId, juce::MidiMessage::getMidiNoteInHertz(key));
            dsp.setVoiceParamValue("/FormantSynth/gate", monoVoiceId, 1);
            dsp.setVoiceParamValue("/FormantSynth/gain", monoVoiceId, velocity);
        }
    }
}

void FormantSynthAudioProcessor::keyOff(int key)
{
    // If statement to switch between monophonic/polyphonic voice destruction
    if (1) {  // If polyphonic. Always true; mono not fully implemented
        dsp.keyOff(key);  // Destroy DSP voice associated with key
    }
    else {  // If monophonic
        dsp.setVoiceParamValue("/FormantSynth/gate", monoVoiceId, 0);  // Turn off mono voice gate
    }
}
/*DSP Setters*/
void FormantSynthAudioProcessor::setBpSourceWave()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/bpSourceSelect", *apvts.getRawParameterValue(SOURCE_WAVE_ID));
}
void FormantSynthAudioProcessor::setBpSourcePw()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/bpSourcePW", *apvts.getRawParameterValue(SOURCE_PW_ID));
}
void FormantSynthAudioProcessor::setBpSourcePressure()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/Pressure", *apvts.getRawParameterValue(SOURCE_PRESSURE_ID));
}
void FormantSynthAudioProcessor::setBpSourceT0()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/T0", *apvts.getRawParameterValue(SOURCE_T0_ID));
}
void FormantSynthAudioProcessor::setBpSourceTe()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/Te", *apvts.getRawParameterValue(SOURCE_TE_ID));
}
void FormantSynthAudioProcessor::setBpSourceNoise()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/noise", *apvts.getRawParameterValue(SOURCE_NOISE_ID));
}

void FormantSynthAudioProcessor::setFricativeColour()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourLow", *apvts.getRawParameterValue(FRICA_LOWCUT_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourHigh", *apvts.getRawParameterValue(FRICA_HIGHCUT_ID));
}

void FormantSynthAudioProcessor::setFricativeLowCut()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourLow", *apvts.getRawParameterValue(FRICA_LOWCUT_ID));
}

void FormantSynthAudioProcessor::setFricativeHighCut()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourHigh", *apvts.getRawParameterValue(FRICA_HIGHCUT_ID));
}

void FormantSynthAudioProcessor::setF1Freq()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f1Freq", *apvts.getRawParameterValue(F1_FREQ_ID));
}
void FormantSynthAudioProcessor::setF1Bandwidth()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f1BW", *apvts.getRawParameterValue(F1_BW_ID));
}
void FormantSynthAudioProcessor::setF1Gain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f1Gain", *apvts.getRawParameterValue(F1_GAIN_ID));
}

void FormantSynthAudioProcessor::setF2Freq()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f2Freq", *apvts.getRawParameterValue(F2_FREQ_ID));
}
void FormantSynthAudioProcessor::setF2Bandwidth()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f2BW", *apvts.getRawParameterValue(F2_BW_ID));
}
void FormantSynthAudioProcessor::setF2Gain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f2Gain", *apvts.getRawParameterValue(F2_GAIN_ID));
}

void FormantSynthAudioProcessor::setF3Freq()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f3Freq", *apvts.getRawParameterValue(F3_FREQ_ID));
}
void FormantSynthAudioProcessor::setF3Bandwidth()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f3BW", *apvts.getRawParameterValue(F3_BW_ID));
}
void FormantSynthAudioProcessor::setF3Gain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f3Gain", *apvts.getRawParameterValue(F3_GAIN_ID));
}

void FormantSynthAudioProcessor::setF4Freq()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f4Freq", *apvts.getRawParameterValue(F4_FREQ_ID));
}
void FormantSynthAudioProcessor::setF4Bandwidth()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f4BW", *apvts.getRawParameterValue(F4_BW_ID));
}
void FormantSynthAudioProcessor::setF4Gain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f4Gain", *apvts.getRawParameterValue(F4_GAIN_ID));
}

void FormantSynthAudioProcessor::setF5Freq()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f5Freq", *apvts.getRawParameterValue(F5_FREQ_ID));
}
void FormantSynthAudioProcessor::setF5Bandwidth()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f5BW", *apvts.getRawParameterValue(F5_BW_ID));
}
void FormantSynthAudioProcessor::setF5Gain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f5Gain", *apvts.getRawParameterValue(F5_GAIN_ID));
}

void FormantSynthAudioProcessor::setVoiceAttack()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceAttack", *apvts.getRawParameterValue(VOICE_ATTACK_ID));
}
void FormantSynthAudioProcessor::setVoiceDecay()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceDecay", *apvts.getRawParameterValue(VOICE_DECAY_ID));
}
void FormantSynthAudioProcessor::setVoiceSustain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceSustain", *apvts.getRawParameterValue(VOICE_SUSTAIN_ID));
}
void FormantSynthAudioProcessor::setVoiceRelease()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceRelease", *apvts.getRawParameterValue(VOICE_RELEASE_ID));
}

void FormantSynthAudioProcessor::setFricativeAttack()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseAttack", *apvts.getRawParameterValue(FRICA_ATTACK_ID));
}
void FormantSynthAudioProcessor::setFricativeDecay()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseDecay", *apvts.getRawParameterValue(FRICA_DECAY_ID));
}
void FormantSynthAudioProcessor::setFricativeSustain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseSustain", *apvts.getRawParameterValue(FRICA_SUSTAIN_ID));
}
void FormantSynthAudioProcessor::setFricativeRelease()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseRelease", *apvts.getRawParameterValue(FRICA_RELEASE_ID));
}

void FormantSynthAudioProcessor::setVibratoFrequency()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoFreq", *apvts.getRawParameterValue(VIBRATO_FREQUENCY_ID));
}
void FormantSynthAudioProcessor::setVibratoAttack()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoAttack", *apvts.getRawParameterValue(VIBRATO_ATTACK_ID));
}
void FormantSynthAudioProcessor::setVibratoSustain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoSustain", *apvts.getRawParameterValue(VIBRATO_SUSTAIN_ID));
}
void FormantSynthAudioProcessor::setVibratoRelease()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoRelease", *apvts.getRawParameterValue(VIBRATO_RELEASE_ID));
}

void FormantSynthAudioProcessor::setFofGain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/fofGain", *apvts.getRawParameterValue(FOF_GAIN_ID));
}

void FormantSynthAudioProcessor::setBpGain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/bpGain", *apvts.getRawParameterValue(BP_GAIN_ID));
}

void FormantSynthAudioProcessor::setFricativeGain()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/fricativeGain", *apvts.getRawParameterValue(FRICA_GAIN_ID));
}

void FormantSynthAudioProcessor::addPhonemeToVector(Phoneme p)
{
    phonemeVector.push_back(std::move(p));
}

void FormantSynthAudioProcessor::setVowelNumber()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/vowelNumber", *apvts.getRawParameterValue(PHONEME_ID));
}

void FormantSynthAudioProcessor::setSkirtWidth()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/useSkirtWidthMult", *apvts.getRawParameterValue(SKIRTWIDTH_ID));
}

void FormantSynthAudioProcessor::setPhoneme(std::vector<Phoneme> pVector, float val)
{
    int nPhonemes = pVector.size();  // Get number of phonemes stored in phoneme vector
    float interpolationVal = val / apvts.getParameterRange(PHONEME_ID).end;  // Map interpolation value to (0.0f - 1.0f)

    float scaledInterpolationValue = interpolationVal * (nPhonemes-1);  // Scale interpolation value so each phoneme occurs at integer value
    float interpolationOff = 0;  // Float to store interpolation offset (integer part of interpolation value)
    float normalisedInterpolationValue = std::modf(scaledInterpolationValue, &interpolationOff);  // Gets floating part of interpolation value
    int integralOffset = static_cast<int>(interpolationOff);  // Cast interpolation offset to integer
    if (integralOffset == nPhonemes - 1) {  // If last phoneme is selected
        interpolatedPhoneme = interpolatePhonemes(phonemeVector[integralOffset], phonemeVector[integralOffset], 0);  // interpolated phoneme = last phoneme
        // Prevents out of bounds error by interpolating beyond end of vector
    }
    else {
        // Interpolate phonemes
        interpolatedPhoneme = interpolatePhonemes(phonemeVector[integralOffset], phonemeVector[integralOffset + 1], normalisedInterpolationValue);
    }
    // Set DSP parameters to new phoneme values
    setF1Freq();
    setF1Bandwidth();
    setF1Gain();
    setF2Freq();
    setF2Bandwidth();
    setF2Gain();
    setF3Freq();
    setF3Bandwidth();
    setF3Gain();
    setF4Freq();
    setF4Bandwidth();
    setF4Gain();
    setF5Freq();
    setF5Bandwidth();
    setF5Gain();
    setFricativeLowCut();
    setFricativeHighCut();
    setFricativeAttack();
    setFricativeDecay();
    setFricativeSustain();
    setFricativeRelease();
    // Set vowel number for skirt width multiplication
    setVowelNumber();
    // Do not update mixer gains if gains are locked
    if (*apvts.getRawParameterValue(FRICA_LOCK_ID) != 1) { setFricativeGain(); }
    if (*apvts.getRawParameterValue(FOF_LOCK_ID) != 1) { setFofGain(); }
    if (*apvts.getRawParameterValue(BP_LOCK_ID) != 1) { setBpGain(); }
    
}

Phoneme FormantSynthAudioProcessor::interpolatePhonemes(Phoneme p1, Phoneme p2, float val)
{
    Phoneme tempPhoneme;  // Temp phoneme object
    if (val == 0.0) {  // If phoneme is directly selected
        tempPhoneme.setName(p1.getName());  // Set display name to phoneme name
    }
    else {  // If point between phonemes is selected
        tempPhoneme.setName(p1.getName() + "->" + p2.getName());  // Set display name to demonstrate phonemes have been interpolated
    }
    for (int i = 0; i < 5; i++) {  // Loop through formants in phoneme
        // Set frequency, bandwidth, gain of temp phoneme to interpolated values
        tempPhoneme.setFrequency(i, (lerp(p1.getFrequency(i), p2.getFrequency(i), val) + *apvts.getRawParameterValue(FORMANT_SHIFT_ID)));
        tempPhoneme.setBandwidth(i, lerp(p1.getBandwidth(i), p2.getBandwidth(i), val));
        tempPhoneme.setGain(i, lerp(p1.getGain(i), p2.getGain(i), val));
    }
    tempPhoneme.setFofGain(lerp(p1.getFofGain(), p2.getFofGain(), val));  // Set FOF gain to interpolated value
    tempPhoneme.setBpGain(lerp(p1.getBpGain(), p2.getBpGain(), val));  // Set bandpass gain to interpolated value
    tempPhoneme.setFricativeColour(lerp(p1.getFricativeLow(), p2.getFricativeLow(), val),
        lerp(p1.getFricativeHigh(), p2.getFricativeHigh(), val));  // Set fricative bandpass filter to interpolated values
    tempPhoneme.setFricativeGain(lerp(p1.getFricativeGain(), p2.getFricativeGain(), val));  // Set fricative gain to interpolated value
    tempPhoneme.setFricativeAttack(lerp(p1.getFricativeAttack(), p2.getFricativeAttack(), val));  // Set fricative attack to interpolated value
    tempPhoneme.setFricativeDecay(lerp(p1.getFricativeDecay(), p2.getFricativeDecay(), val));  // Set fricative decay to interpolated value
    tempPhoneme.setFricativeSustain(lerp(p1.getFricativeSustain(), p2.getFricativeSustain(), val));  // Set fricative sustain to interpolated value
    tempPhoneme.setFricativeRelease(lerp(p1.getFricativeRelease(), p2.getFricativeRelease(), val));  // Set fricative release to interpolated value
    return tempPhoneme;
}

float FormantSynthAudioProcessor::lerp(float a, float b, float t)
{
    return (a + (t*(b - a)));  // Return interpolated value
}

void FormantSynthAudioProcessor::loadButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select Phoneme Data File...", juce::File{}, "*.xml");  // Create file browser dialog, filter for XML files
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;  // File browser configuration
    
    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)  // Launch file browser window
        {
            auto file = fc.getResult();  // Store selected file in auto object
            if (file != juce::File{}) {  // If a file has been selected
                auto xml = juce::parseXML(file);  // Parse file into XML tree
                if (xml->hasTagName("PHONEME_DATA")) {  // If XML file is a phoneme data file
                    phonemeVector.clear();  // Empty phoneme vector
                    for (auto* phoneme : xml->getChildIterator()) {  // For every child element in XML file (every phoneme)
                        Phoneme tp;  // Temp phoneme object
                        juce::String tpName = phoneme->getTagName();  // Get phoneme name from file
                        tp.setName(tpName.toStdString());  // Set temp phoneme name to name from file
                        for (auto* f : phoneme->getChildIterator()) {  // For every child element of phoneme (every formant, fricative)
                            if (f->hasTagName("F1")) {  // If the child data corresponds to formant 1
                                tp.setFrequency(0, f->getDoubleAttribute("Freq"));  // Set phoneme formant data to data from child
                                tp.setBandwidth(0, f->getDoubleAttribute("Bandwidth"));
                                tp.setGain(0, f->getDoubleAttribute("Gain"));
                            }
                            if (f->hasTagName("F2")) {  // If the child data corresponds to formant 2
                                tp.setFrequency(1, f->getDoubleAttribute("Freq"));  // Set phoneme formant data to data from child
                                tp.setBandwidth(1, f->getDoubleAttribute("Bandwidth"));
                                tp.setGain(1, f->getDoubleAttribute("Gain"));
                            }
                            if (f->hasTagName("F3")) {  // If the child data corresponds to formant 3
                                tp.setFrequency(2, f->getDoubleAttribute("Freq"));  // Set phoneme formant data to data from child
                                tp.setBandwidth(2, f->getDoubleAttribute("Bandwidth"));
                                tp.setGain(2, f->getDoubleAttribute("Gain"));
                            }
                            if (f->hasTagName("F4")) {  // If the child data corresponds to formant 4
                                tp.setFrequency(3, f->getDoubleAttribute("Freq"));  // Set phoneme formant data to data from child
                                tp.setBandwidth(3, f->getDoubleAttribute("Bandwidth"));
                                tp.setGain(3, f->getDoubleAttribute("Gain"));
                            }
                            if (f->hasTagName("F5")) {  // If the child data corresponds to formant 5
                                tp.setFrequency(4, f->getDoubleAttribute("Freq"));  // Set phoneme formant data to data from child
                                tp.setBandwidth(4, f->getDoubleAttribute("Bandwidth"));
                                tp.setGain(4, f->getDoubleAttribute("Gain"));
                            }
                            if (f->hasTagName("FRICATIVE")) {  // If the child data corresponds to fricative data
                                tp.setFricativeGain(f->getDoubleAttribute("Gain"));  // Set phoneme fricative data to data from child
                                tp.setFricativeColour(f->getDoubleAttribute("Lowcut"), f->getDoubleAttribute("Highcut"));
                                tp.setFricativeAttack(f->getDoubleAttribute("Attack"));
                                tp.setFricativeDecay(f->getDoubleAttribute("Decay"));
                                tp.setFricativeSustain(f->getDoubleAttribute("Sustain"));
                                tp.setFricativeRelease(f->getDoubleAttribute("Release"));
                            }
                            if (f->hasTagName("VOICED")) {  // If the child data corresponds to voiced data
                                tp.setFofGain(f->getDoubleAttribute("FOF_gain"));  // Set phoneme voiced data to data from child
                                tp.setBpGain(f->getDoubleAttribute("BP_gain"));
                            }
                        }
                        phonemeVector.push_back(std::move(tp));  // Add temp phoneme to phoneme vector
                    }
                }
                
            }
        });
}
