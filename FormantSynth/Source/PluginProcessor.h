/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <math.h>
#include "DspFaust.h"

//==============================================================================
/**
*/
class FormantSynthAudioProcessor  : public juce::AudioProcessor
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

    void setFormantFreqs(int vowel);
    void setFormantBandwidths(int vowel);
    void setFormantGains(int vowel);
    void setInFreq(int freq);

private:
    int formantFreqData[5][5] =
    {
        {800, 1150, 2800, 3500, 4950},
        {400, 1600, 2700, 3300, 4950},
        {350, 1700, 2700, 3700, 4950},
        {450, 800, 2830, 3500, 4950},
        {325, 700, 2530, 3500, 4950}
    };
    int formantGainData[5][5] =
    {
        {0, -4, -20, -36, -60},
        {0, -24, -30, -35, -60},
        {0, -20, -30, -36, -60},
        {0, -9, -16, -28, -55},
        {0, -12, -30, -40, -64}
    };
    int formantBWData[5][5] =
    {
        {80, 90, 120, 130, 140},
        {60, 80, 120, 150, 200},
        {50, 100, 120, 150, 200},
        {70, 80, 100, 130, 135},
        {50, 60, 170, 180, 200}
    };

    DspFaust dspFaust;

    float dbToGain(int dB);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessor)
};
