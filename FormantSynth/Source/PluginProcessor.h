/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspFaust.h"



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
    void keyOn(int key, int velocity);
    void keyOff(int key);

    void setBpSourceWave(int value);
    void setBpSourcePw(float pw);
    void setBpSourcePressure(float pressure);
    void setBpSourceT0(float t0);
    void setBpSourceTe(float te);
    void setBpSourceNoise(float noise);

    void setFricativeColour(float min, float max);

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

    float getCpuLoad();

private:
    DspFaust dsp;  // Main DSP object

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantSynthAudioProcessor)
};
