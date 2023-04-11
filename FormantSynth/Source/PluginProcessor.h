#pragma once

#include <JuceHeader.h>
#include "DspFaust.h"
#include "Phoneme.h"

#define SOURCE_PW_ID "bpSourcePw"
#define SOURCE_WAVE_ID "bpSourceWave"
#define SOURCE_T0_ID "sourceT0"
#define SOURCE_TE_ID "sourceTe"
#define SOURCE_NOISE_ID "sourceNoise"
#define SOURCE_PRESSURE_ID "sourcePressure"
#define FRICA_LOWCUT_ID "fricativeLowCut"
#define FRICA_HIGHCUT_ID "fricativeHighCut"
#define MONO_ID "monoSelect"

#define PHONEME_ID "phoneme"
#define F1_FREQ_ID "f1Freq"
#define F1_GAIN_ID "f1Gain"
#define F1_BW_ID "f1Bandwidth"
#define F2_FREQ_ID "f2Freq"
#define F2_GAIN_ID "f2Gain"
#define F2_BW_ID "f2Bandwidth"
#define F3_FREQ_ID "f3Freq"
#define F3_GAIN_ID "f3Gain"
#define F3_BW_ID "f3Bandwidth"
#define F4_FREQ_ID "f4Freq"
#define F4_GAIN_ID "f4Gain"
#define F4_BW_ID "f4Bandwidth"
#define F5_FREQ_ID "f5Freq"
#define F5_GAIN_ID "f5Gain"
#define F5_BW_ID "f5Bandwidth"

#define VIBRATO_FREQUENCY_ID "vibratoFreq"
#define VIBRATO_ATTACK_ID "vibratoAttack"
#define VIBRATO_SUSTAIN_ID "vibratoSustain"
#define VIBRATO_RELEASE_ID "vibratoRelease"

#define VOICE_ATTACK_ID "voiceAttack"
#define VOICE_DECAY_ID "voiceDecay"
#define VOICE_SUSTAIN_ID "voiceSustain"
#define VOICE_RELEASE_ID "voiceRelease"

#define FRICA_ATTACK_ID "fricativeAttack"
#define FRICA_DECAY_ID "fricativeDecay"
#define FRICA_SUSTAIN_ID "fricativeSustain"
#define FRICA_RELEASE_ID "fricativeRelease"

#define FOF_GAIN_ID "fofGain"
#define BP_GAIN_ID "bpGain"
#define FRICA_GAIN_ID "fricativeGain"

//==============================================================================
/**
*/
class FormantSynthAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    FormantSynthAudioProcessor();
    ~FormantSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Note on/off control
    void keyOn(int key, int velocity);
    void keyOff(int key);
    uintptr_t monoVoiceId = 0;

    // Parameter control functions
    void setBpSourceWave(int value);
    void setBpSourcePw(float pw);
    void setBpSourcePressure(float pressure);
    void setBpSourceT0(float t0);
    void setBpSourceTe(float te);
    void setBpSourceNoise(float noise);

    void setFricativeColour(float min, float max);

    void setF1Freq(float freq);
    void setF1Bandwidth(float bandwidth);
    void setF1Gain(float gain);

    void setF2Freq(float freq);
    void setF2Bandwidth(float bandwidth);
    void setF2Gain(float gain);

    void setF3Freq(float freq);
    void setF3Bandwidth(float bandwidth);
    void setF3Gain(float gain);

    void setF4Freq(float freq);
    void setF4Bandwidth(float bandwidth);
    void setF4Gain(float gain);

    void setF5Freq(float freq);
    void setF5Bandwidth(float bandwidth);
    void setF5Gain(float gain);

    void setVoiceAttack(float attack);
    void setVoiceDecay(float decay);
    void setVoiceSustain(float sustain);
    void setVoiceRelease(float release);

    void setFricativeAttack(float attack);
    void setFricativeDecay(float decay);
    void setFricativeSustain(float sustain);
    void setFricativeRelease(float release);

    void setVibratoFrequency(float frequency);
    void setVibratoAttack(float attack);
    void setVibratoSustain(float sustain);
    void setVibratoRelease(float release);

    void setFofGain(float gain);
    void setBpGain(float gain);
    void setFricativeGain(float gain);

    void setPhoneme(std::vector<Phoneme> pVector, float interpolationVal);
    Phoneme interpolatePhonemes(Phoneme p1, Phoneme p2, float val);
    float lerp(float a, float b, float t);

    float getCpuLoad();

    // Value Tree
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // File Parsing
    std::vector<Phoneme> phonemeVector;
    
    Phoneme interpolatedPhoneme;

private:
    DspFaust dsp;  // Main DSP object

    void updateDspFromParameters();

    // Phonemes
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessor)
};
