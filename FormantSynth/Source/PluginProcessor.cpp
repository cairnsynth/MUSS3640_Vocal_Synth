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

void FormantSynthAudioProcessor::setBpSourceWave(int value)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/bpSourceSelect", value-1);
    DBG(value);
}

void FormantSynthAudioProcessor::setBpSourcePwValue(float pw)
{
    dsp.setParamValue("/Polyphonic/Voices/FormantSynth/voice/source/bpSourcePW", pw);
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