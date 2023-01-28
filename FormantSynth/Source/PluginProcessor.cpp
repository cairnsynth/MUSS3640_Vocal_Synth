/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    dspFaust.start();
}

FormantSynthAudioProcessor::~FormantSynthAudioProcessor()
{
    dspFaust.stop();
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    FormantSynthAudioProcessor::setFormantFreqs(2);
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

float FormantSynthAudioProcessor::dbToGain(int dB)
{
    return pow(10.0, (float)dB / 20.0);
}

void FormantSynthAudioProcessor::setFormantFreqs(int vowel)
{
    dspFaust.setParamValue("f1Freq", formantFreqData[vowel][0]);
    dspFaust.setParamValue("f2Freq", formantFreqData[vowel][1]);
    dspFaust.setParamValue("f3Freq", formantFreqData[vowel][2]);
    dspFaust.setParamValue("f4Freq", formantFreqData[vowel][3]);
    dspFaust.setParamValue("f5Freq", formantFreqData[vowel][4]);
}

void FormantSynthAudioProcessor::setFormantGains(int vowel)
{
    dspFaust.setParamValue("f1Gain", dbToGain(formantGainData[vowel][0]));
    dspFaust.setParamValue("f2Gain", dbToGain(formantGainData[vowel][1]));
    dspFaust.setParamValue("f3Gain", dbToGain(formantGainData[vowel][2]));
    dspFaust.setParamValue("f4Gain", dbToGain(formantGainData[vowel][3]));
    dspFaust.setParamValue("f5Gain", dbToGain(formantGainData[vowel][4]));
}

void FormantSynthAudioProcessor::setFormantBandwidths(int vowel)
{
    dspFaust.setParamValue("f1BW", formantBWData[vowel][0]);
    dspFaust.setParamValue("f2BW", formantBWData[vowel][1]);
    dspFaust.setParamValue("f3BW", formantBWData[vowel][2]);
    dspFaust.setParamValue("f4BW", formantBWData[vowel][3]);
    dspFaust.setParamValue("f5BW", formantBWData[vowel][4]);
}

void FormantSynthAudioProcessor::setInFreq(int freq)
{
    dspFaust.setParamValue("freq", freq);
}