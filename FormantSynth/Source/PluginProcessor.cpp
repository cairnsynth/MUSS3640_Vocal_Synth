#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FormantSynthAudioProcessor::FormantSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
        #if ! JucePlugin_IsMidiEffect
        #if ! JucePlugin_IsSynth
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
        #endif
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)
        #endif
            ),
apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    Phoneme defaultA;
    defaultA.setFormant(0, 600, 60, 1.0);
    defaultA.setFormant(1, 1040, 70, 0.199526);
    defaultA.setFormant(2, 2250, 110, 0.199526);
    defaultA.setFormant(3, 2450, 120, 0.199526);
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

    phonemeVector.push_back(std::move(defaultA));
    phonemeVector.push_back(std::move(defaultE));
    phonemeVector.push_back(std::move(defaultI));
    phonemeVector.push_back(std::move(defaultO));
    phonemeVector.push_back(std::move(defaultU));


    dsp.start();
}

FormantSynthAudioProcessor::~FormantSynthAudioProcessor()
{
    dsp.stop();
}

//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout FormantSynthAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    // Source parameter creation
    auto sourceWaveParam = std::make_unique<juce::AudioParameterInt>(SOURCE_WAVE_ID, "Source Waveform", 1, 3, 1);
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
    // Filter parameter creation
    auto phonemeParam = std::make_unique<juce::AudioParameterFloat>(PHONEME_ID, "Phoneme", 0.0, 10.0, 0.0);
    params.push_back(std::move(phonemeParam));

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
    auto bpGainParam = std::make_unique<juce::AudioParameterFloat>(BP_GAIN_ID, "Bandpass Model Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(bpGainParam));
    auto fricativeGainParam = std::make_unique<juce::AudioParameterFloat>(FRICA_GAIN_ID, "Fricative Gain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(fricativeGainParam));

    return { params.begin(), params.end() };
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

void FormantSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FormantSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FormantSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FormantSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void FormantSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FormantSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
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
    updateDspFromParameters();
}

//==============================================================================
bool FormantSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FormantSynthAudioProcessor::createEditor()
{
    return new FormantSynthAudioProcessorEditor (*this);
}

//==============================================================================
void FormantSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FormantSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FormantSynthAudioProcessor();
}

//==============================================================================
//End JUCE generated Code
void FormantSynthAudioProcessor::updateDspFromParameters()
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/bpSourceSelect", *apvts.getRawParameterValue(SOURCE_WAVE_ID) - 1);
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/bpSourcePW", *apvts.getRawParameterValue(SOURCE_PW_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/Pressure", *apvts.getRawParameterValue(SOURCE_PRESSURE_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/T0", *apvts.getRawParameterValue(SOURCE_T0_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/Te", *apvts.getRawParameterValue(SOURCE_TE_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/noise", *apvts.getRawParameterValue(SOURCE_NOISE_ID));

    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoFreq", *apvts.getRawParameterValue(VIBRATO_FREQUENCY_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoAttack", *apvts.getRawParameterValue(VIBRATO_ATTACK_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoSustain", *apvts.getRawParameterValue(VIBRATO_SUSTAIN_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoRelease", *apvts.getRawParameterValue(VIBRATO_RELEASE_ID));

    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceAttack", *apvts.getRawParameterValue(VOICE_ATTACK_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceDecay", *apvts.getRawParameterValue(VOICE_DECAY_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceSustain", *apvts.getRawParameterValue(VOICE_SUSTAIN_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceRelease", *apvts.getRawParameterValue(VOICE_RELEASE_ID));

    //dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourHigh", *apvts.getRawParameterValue(FRICA_HIGHCUT_ID));
    //dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourLow", *apvts.getRawParameterValue(FRICA_LOWCUT_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseAttack", *apvts.getRawParameterValue(FRICA_ATTACK_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseDecay", *apvts.getRawParameterValue(FRICA_DECAY_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseSustain", *apvts.getRawParameterValue(FRICA_SUSTAIN_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseRelease", *apvts.getRawParameterValue(FRICA_RELEASE_ID));

    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/fofGain", *apvts.getRawParameterValue(FOF_GAIN_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/bpGain", *apvts.getRawParameterValue(BP_GAIN_ID));
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/fricativeGain", *apvts.getRawParameterValue(FRICA_GAIN_ID));

    //setPhoneme(phonemeVector[*apvts.getRawParameterValue(MONO_ID)]);
}

void FormantSynthAudioProcessor::keyOn(int key, int velocity)
{
    //MONO NOT IMPLEMENTED
    if (1) {
        dsp.keyOn(key, velocity);
    }
    else {
        if (monoVoiceId == 0) {
            monoVoiceId = dsp.keyOn(key, velocity);
        }
        else {
            dsp.setVoiceParamValue("/FormantSynth/freq", monoVoiceId, juce::MidiMessage::getMidiNoteInHertz(key));
            dsp.setVoiceParamValue("/FormantSynth/gate", monoVoiceId, 1);
            dsp.setVoiceParamValue("/FormantSynth/gain", monoVoiceId, velocity);
        }
    }
}

void FormantSynthAudioProcessor::keyOff(int key)
{
    //MONO NOT IMPLEMENTED
    if (1) {
        dsp.keyOff(key);
    }
    else {
        dsp.setVoiceParamValue("/FormantSynth/gate", monoVoiceId, 0);
    }
}
/*DSP Setters*/
void FormantSynthAudioProcessor::setBpSourceWave(int value)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/bpSourceSelect", value-1);
}
void FormantSynthAudioProcessor::setBpSourcePw(float pw)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/bpSourcePW", pw);
}
void FormantSynthAudioProcessor::setBpSourcePressure(float pressure)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/Pressure", pressure);
}
void FormantSynthAudioProcessor::setBpSourceT0(float t0)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/T0", t0);
}
void FormantSynthAudioProcessor::setBpSourceTe(float te)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/Te", te);
}
void FormantSynthAudioProcessor::setBpSourceNoise(float noise)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/noise", noise);
}

void FormantSynthAudioProcessor::setFricativeColour(float min, float max)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourLow", min);
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourHigh", max);
}

void FormantSynthAudioProcessor::setF1Freq(float freq)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f1Freq", freq);
}
void FormantSynthAudioProcessor::setF1Bandwidth(float bandwidth)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f1BW", bandwidth);
}
void FormantSynthAudioProcessor::setF1Gain(float gain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f1Gain", gain);
}

void FormantSynthAudioProcessor::setF2Freq(float freq)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f2Freq", freq);
}
void FormantSynthAudioProcessor::setF2Bandwidth(float bandwidth)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f2BW", bandwidth);
}
void FormantSynthAudioProcessor::setF2Gain(float gain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f2Gain", gain);
}

void FormantSynthAudioProcessor::setF3Freq(float freq)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f3Freq", freq);
}
void FormantSynthAudioProcessor::setF3Bandwidth(float bandwidth)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f3BW", bandwidth);
}
void FormantSynthAudioProcessor::setF3Gain(float gain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f3Gain", gain);
}

void FormantSynthAudioProcessor::setF4Freq(float freq)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f4Freq", freq);
}
void FormantSynthAudioProcessor::setF4Bandwidth(float bandwidth)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f4BW", bandwidth);
}
void FormantSynthAudioProcessor::setF4Gain(float gain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f4Gain", gain);
}

void FormantSynthAudioProcessor::setF5Freq(float freq)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f5Freq", freq);
}
void FormantSynthAudioProcessor::setF5Bandwidth(float bandwidth)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f5BW", bandwidth);
}
void FormantSynthAudioProcessor::setF5Gain(float gain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/f5Gain", gain);
}

void FormantSynthAudioProcessor::setVoiceAttack(float attack)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceAttack", attack);
}
void FormantSynthAudioProcessor::setVoiceDecay(float decay)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceDecay", decay);
}
void FormantSynthAudioProcessor::setVoiceSustain(float sustain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceSustain", sustain);
}
void FormantSynthAudioProcessor::setVoiceRelease(float release)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/envelope/voiceRelease", release);
}

void FormantSynthAudioProcessor::setFricativeAttack(float attack)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseAttack", attack);
}
void FormantSynthAudioProcessor::setFricativeDecay(float decay)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseDecay", decay);
}
void FormantSynthAudioProcessor::setFricativeSustain(float sustain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseSustain", sustain);
}
void FormantSynthAudioProcessor::setFricativeRelease(float release)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/fricative/noiseRelease", release);
}

void FormantSynthAudioProcessor::setVibratoFrequency(float frequency)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoFreq", frequency);
}
void FormantSynthAudioProcessor::setVibratoAttack(float attack)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoAttack", attack);
}
void FormantSynthAudioProcessor::setVibratoSustain(float sustain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoSustain", sustain);
}
void FormantSynthAudioProcessor::setVibratoRelease(float release)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoRelease", release);
}

void FormantSynthAudioProcessor::setFofGain(float gain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/fofGain", gain);
}

void FormantSynthAudioProcessor::setBpGain(float gain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/bpGain", gain);
}

void FormantSynthAudioProcessor::setFricativeGain(float gain)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/mixer/fricativeGain", gain);
}

float FormantSynthAudioProcessor::getCpuLoad()
{
    return dsp.getCPULoad();
}

void FormantSynthAudioProcessor::setPhoneme(std::vector<Phoneme> pVector, float val)
{
    int nPhonemes = pVector.size();
    float interpolationVal = val / apvts.getParameterRange(PHONEME_ID).end;

    if (nPhonemes == 1) {
        DBG("1 phoneme");
    }
    float phonemeSpacing = interpolationVal / (nPhonemes-1);
    float scaledInterpolationValue = interpolationVal * (nPhonemes-1);
    float interpolationOff = 0;
    float normalisedInterpolationValue = std::modf(scaledInterpolationValue, &interpolationOff);
    int integralOffset = static_cast<int>(interpolationOff);
    if (integralOffset == nPhonemes - 1) {
        interpolatedPhoneme = phonemeVector[integralOffset];
    }
    else {
        interpolatedPhoneme = interpolatePhonemes(phonemeVector[integralOffset], phonemeVector[integralOffset + 1], normalisedInterpolationValue);
    }

    setF1Freq(*apvts.getRawParameterValue(F1_FREQ_ID));
    setF1Bandwidth(*apvts.getRawParameterValue(F1_BW_ID));
    setF1Gain(*apvts.getRawParameterValue(F1_GAIN_ID));
    setF2Freq(*apvts.getRawParameterValue(F2_FREQ_ID));
    setF2Bandwidth(*apvts.getRawParameterValue(F2_BW_ID));
    setF2Gain(*apvts.getRawParameterValue(F2_GAIN_ID));
    setF3Freq(*apvts.getRawParameterValue(F3_FREQ_ID));
    setF3Bandwidth(*apvts.getRawParameterValue(F3_BW_ID));
    setF3Gain(*apvts.getRawParameterValue(F3_GAIN_ID));
    setF4Freq(*apvts.getRawParameterValue(F4_FREQ_ID));
    setF4Bandwidth(*apvts.getRawParameterValue(F4_BW_ID));
    setF4Gain(*apvts.getRawParameterValue(F4_GAIN_ID));
    setF5Freq(*apvts.getRawParameterValue(F5_FREQ_ID));
    setF5Bandwidth(*apvts.getRawParameterValue(F5_BW_ID));
    setF5Gain(*apvts.getRawParameterValue(F5_GAIN_ID));

    DBG(interpolatedPhoneme.getName());
}

Phoneme FormantSynthAudioProcessor::interpolatePhonemes(Phoneme p1, Phoneme p2, float val)
{
    Phoneme tempPhoneme;
    if (val == 0.0) {
        tempPhoneme.setName(p1.getName());
    }
    else {
        tempPhoneme.setName(p1.getName() + "->" + p2.getName());
    }
    for (int i = 0; i < 5; i++) {
        tempPhoneme.setFrequency(i, lerp(p1.getFrequency(i), p2.getFrequency(i), val));
        tempPhoneme.setBandwidth(i, lerp(p1.getBandwidth(i), p2.getBandwidth(i), val));
        tempPhoneme.setGain(i, lerp(p1.getGain(i), p2.getGain(i), val));
    }
    tempPhoneme.setFricativeColour(lerp(p1.getFricativeLow(), p2.getFricativeLow(), val),
        lerp(p1.getFricativeHigh(), p2.getFricativeHigh(), val));
    tempPhoneme.setFricativeGain(lerp(p1.getFricativeGain(), p2.getFricativeGain(), val));
    tempPhoneme.setFricativeAttack(lerp(p1.getFricativeAttack(), p2.getFricativeAttack(), val));
    tempPhoneme.setFricativeDecay(lerp(p1.getFricativeDecay(), p2.getFricativeDecay(), val));
    tempPhoneme.setFricativeSustain(lerp(p1.getFricativeSustain(), p2.getFricativeSustain(), val));
    tempPhoneme.setFricativeRelease(lerp(p1.getFricativeRelease(), p2.getFricativeRelease(), val));

    return tempPhoneme;
}

float FormantSynthAudioProcessor::lerp(float a, float b, float t)
{
    return (a + (t*(b - a)));
}