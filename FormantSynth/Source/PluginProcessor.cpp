/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

//==============================================================================
FormantSynthAudioProcessor::FormantSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    dsp.start();
}

FormantSynthAudioProcessor::~FormantSynthAudioProcessor()
{
    dsp.stop();
}

//==============================================================================
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
//End JUCE Code
void FormantSynthAudioProcessor::keyOn(int key, int velocity)
{
    uintptr_t voiceAddress = dsp.keyOn(key, velocity);
}

void FormantSynthAudioProcessor::keyOff(int key)
{
    dsp.keyOff(key);
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