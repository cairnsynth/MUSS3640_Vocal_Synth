#pragma once

#include <JuceHeader.h>
#include "DspFaust.h"
#include "Phoneme.h"
#include "ExternalMidi.h"

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
#define FORMANT_SHIFT_ID "formantShift"
#define SKIRTWIDTH_ID "skirtWidthMult"
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
#define FOF_LOCK_ID "fofGainLock"
#define BP_GAIN_ID "bpGain"
#define BP_LOCK_ID "bpGainLock"
#define FRICA_GAIN_ID "fricativeGain"
#define FRICA_LOCK_ID "fricativeGainLock"

//==============================================================================
/* REF:
*   JUCE MIDILogger example
*/

class FaustPolyEngine;
class audio;

class FormantSynthAudioProcessor  : public juce::AudioProcessor,
    private juce::Timer
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
    void setBpSourceWave();
    void setBpSourcePw();
    void setBpSourcePressure();
    void setBpSourceT0();
    void setBpSourceTe();
    void setBpSourceNoise();

    void setFricativeColour();
    void setFricativeLowCut();
    void setFricativeHighCut();

    void setF1Freq();
    void setF1Bandwidth();
    void setF1Gain();

    void setF2Freq();
    void setF2Bandwidth();
    void setF2Gain();

    void setF3Freq();
    void setF3Bandwidth();
    void setF3Gain();

    void setF4Freq();
    void setF4Bandwidth();
    void setF4Gain();

    void setF5Freq();
    void setF5Bandwidth();
    void setF5Gain();

    void setVoiceAttack();
    void setVoiceDecay();
    void setVoiceSustain();
    void setVoiceRelease();

    void setFricativeAttack();
    void setFricativeDecay();
    void setFricativeSustain();
    void setFricativeRelease();

    void setVibratoFrequency();
    void setVibratoAttack();
    void setVibratoSustain();
    void setVibratoRelease();

    void setFofGain();
    void setBpGain();
    void setFricativeGain();

    void setVowelNumber();
    void setSkirtWidth();
    void addPhonemeToVector(Phoneme p);
    void setPhoneme(std::vector<Phoneme> pVector, float interpolationVal);
    Phoneme interpolatePhonemes(Phoneme p1, Phoneme p2, float val);
    float lerp(float a, float b, float t);

    float getCpuLoad();

    void loadButtonClicked();
    std::unique_ptr<juce::FileChooser> chooser;
    std::unique_ptr<juce::XmlDocument> xmlFile;

    // Value Tree
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void initialisePhonemes();
    // File Parsing
    std::vector<Phoneme> phonemeVector;
    
    Phoneme interpolatedPhoneme;

    juce::MidiKeyboardState keyboardState;

    MidiQueue midiQueue;
    MidiListModel midiModel;

    juce::String midiInfo;

    int noteOnFlag = 0;
    int noteOffFlag = 0;

private:
    DspFaust dsp;  // Main DSP object
    void timerCallback() override;

    void processMidi();
    std::vector<int> voiceKeys;
    //MidiTable midiTable;

    audio* driver;
    FaustPolyEngine* faustObject;
    float** outputs;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessor)
};
