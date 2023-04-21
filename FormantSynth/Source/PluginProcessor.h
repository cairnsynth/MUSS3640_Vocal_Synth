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
    /* Note On/Off control */

    /* --keyOn(int key, int velocity)--
    * Instantiates Faust DSP voice with pitch and velocity data from MIDI message
    *    key = MIDI key value
    *    velocity = MIDI velocity Value
    */
    void keyOn(int key, int velocity);

    /* --keyOff(int key)--
    * Sets gate to 0 for Faust DSP voice associated with given key,
    * waits for end of envelope and destroys voice
    *    key = MIDI key to destroy
    */
    void keyOff(int key);

    uintptr_t monoVoiceId = 0;  // Variable to store voice ID of mono voice

    /* 
    DSP parameter setters
    Sets Faust DSP parameter to value of corresponding value tree parameter
    */
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

    /* --addPhonemeToVector(Phoneme p)--
    * Adds phoneme to end of phonemeVector
    *   p = phoneme to add
    */
    void addPhonemeToVector(Phoneme p);

    /* --setPhoneme(std::vector<Phoneme> pVector, float interpolationVal)--
    * Sets DSP formant frequencies, gains, and bandwidths to values
    * interpolated from pVector
    *   pVector = vector of phonemes from which to get values
    *   interpolationVal = distance through vector to interpolate
    */
    void setPhoneme(std::vector<Phoneme> pVector, float val);

    /* --interpolatePhonemes(Phoneme p1, Phoneme p2, float val)-- 
    * Returns Phoneme object with member values interpolated from p1 and p2
    *   p1 = starting phoneme
    *   p2 = ending phoneme
    *   val = interpolation point (0.0f - 1.0f)
    */
    Phoneme interpolatePhonemes(Phoneme p1, Phoneme p2, float val);

    /* --lerp(float a, floatb, float t)--
    * Basic linear interpolation function
    * Used in interpolatePhonemes()
    *   a = starting value
    *   b = ending value
    *   t = interpolation point
    * 
    * REFERENCE:
    *   cppreference.com, "std::lerp",
    *   <https://en.cppreference.com/w/cpp/numeric/lerp>
    *   [accessed 21/04/2023]
    */
    float lerp(float a, float b, float t);

    /* --loadButtonClicked()--
    * Function handles loading XML files containing phonemes into
    * phoneme vector
    * 
    * REFERENCE: (for file browser dialog)
    *   Juce, "Tutorial: Build an audio player",
    *   <https://docs.juce.com/master/tutorial_playing_sound_files.html#tutorial_playing_sound_files_opening_a_file>
    *   [accessed 21/04/2023]
    */
    void loadButtonClicked();
    std::unique_ptr<juce::FileChooser> chooser;  // File Explorer dialog
    std::unique_ptr<juce::XmlDocument> xmlFile;  // XML file container to load in to

    /* --Audio Processor Value Tree State--
    * Stores values of each parameter in the plugin processor allowing DAW 
    * control/automation
    */
    juce::AudioProcessorValueTreeState apvts;

    /* --createParameterLayout()--
    * Creates parameter objects for each parameter, 
    * defining range and intial value
    */
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    /* --initialisePhonemes()-- 
    * Loads default phoneme values into phoneme vector
    * Hard coded to avoid error in the event of missing XML files
    */
    void initialisePhonemes();
    
    std::vector<Phoneme> phonemeVector;  // Vector stores loaded phoneme objects
    
    Phoneme interpolatedPhoneme;  // 'Current' phoneme

    juce::MidiKeyboardState keyboardState;  // GUI keyboard state

    MidiQueue midiQueue;  // Queue of incomming MIDI messages
    MidiListModel midiModel;  // List of MIDI messages to be acted on by plugin

private:
    DspFaust dsp;  // Main Faust DSP object

    /* --timerCallBack()-- 
    * Function called when timer triggers
    * Updates midiQueue, midiModel with new external MIDI messages
    * 
    * REFERENCE:
    *   tpoole, "MidiLoggerPluginDemo.h", 
    *   <https://github.com/juce-framework/JUCE/blob/master/examples/Plugins/MidiLoggerPluginDemo.h>
    *   [accessed 21/04/2023]
    */
    void timerCallback() override;

    /* --processMidi()-- 
    * Processes external MIDI inputs, creating DSP voices for new key presses
    * Prevents creation of multiple voices on the same key, which may lead to
    * errors on the destruction of DSP voices
    */
    void processMidi();
    std::vector<int> voiceKeys;  // Stores currently active keys

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessor)
};
