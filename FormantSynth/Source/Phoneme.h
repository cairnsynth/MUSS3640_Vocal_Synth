/*
  ==============================================================================

    Phoneme.h
    Created: 10 Apr 2023 5:34:13pm
    Author:  Luke Coles

    Class for holding phoneme variables such as frequency, bandwidth, gain

  ==============================================================================
*/
#include <string>
#pragma once

class Phoneme {

public:
    // Formant struct holds formant specific data
    struct Formant {
        float frequency;  // Formant frequency
        float bandwidth;  // Formant bandwidth
        float gain;  // Formant gain
    };
    // Encapsulates Phoneme information
    Phoneme() {
        _name = "New Phoneme";
        _formants[0] = { 0.0f, 0.0f, 0.0f };
        _formants[1] = { 0.0f, 0.0f, 0.0f };
        _formants[2] = { 0.0f, 0.0f, 0.0f };
        _formants[3] = { 0.0f, 0.0f, 0.0f };
        _formants[4] = { 0.0f, 0.0f, 0.0f };
    }
    // Encapsulates Phoneme information
    Phoneme(std::string name, Formant f1, Formant f2, Formant f3, Formant f4, Formant f5) {
        _name = name;
        _formants[0] = f1;
        _formants[1] = f2;
        _formants[2] = f3;
        _formants[3] = f4;
        _formants[4] = f5;
    }
    /*
    setName(std::string name)

    Sets name member variable of phoneme
        name = phoneme name
    */
    void setName(std::string name);
    /*
    setFormant(int formant, float frequency, float bandwidth, float gain)

    Sets data for a single formant
        formant = formant number to set (0 - 4)
        frequency = formant centre frequency (Hz)
        bandwidth = formant bandwidth (Hz)
        gain = formant gain
    */
    void setFormant(int formant, float frequency, float bandwidth, float gain);
    /*
    setFofGain(float gain)

    Sets the gain of the FOF model output
        gain = gain of FOF model (0.0f - 1.0f)
    */
    void setFofGain(float gain);
    /*
    setBpGain(float gain)

    Sets the gain of the bandpass model output
        gain = gain of bandpass model (0.0f - 1.0f)
    */
    void setBpGain(float gain);
    /*
    setFrequency(int formant, float frequency)

    Sets the frequency of a specific formant
        formant = formant number (0 - 4)
        frequency = centre frequency of formant (Hz)
    */
    void setFrequency(int formant, float frequency);
    /*
    setBandwidth(int formant, float bandwidth)

    Sets the bandwidth of a specific formant
        formant = formant number (0 - 4)
        bandwidth = bandwidth of formant (Hz)
    */
    void setBandwidth(int formant, float bandwidth);
    /*
    setGain(int formant, float gain)

    Sets the gain of a specific formant
        formant = formant number (0 - 4)
        gain = gain of formant (0.0f, 1.0f)
    */
    void setGain(int formant, float gain);
    /*
    setFricativeGain(float gain)

    Sets the gain of the phoneme's fricative component
        gain = gain of fricative component (0.0f - 1.0f)
    */
    void setFricativeGain(float gain);
    /*
    setFricativeAttack(float attack)

    Sets the attack of the phoneme's fricative component
        attack = attack of fricative component (seconds)
    */
    void setFricativeAttack(float attack);
    /*
    setFricativeDecay(float decay)

    Sets the decay of the phoneme's fricative component
        decay = decay of fricative component (seconds)
    */
    void setFricativeDecay(float decay);
    /*
    setFricativeSustain(float sustain)

    Sets the sustain of the phoneme's fricative component
        sustain = sustain of fricative component (0.0f - 1.0f)
    */
    void setFricativeSustain(float sustain);
    /*
    setFricativeRelease(float release)

    Sets the release of the phoneme's fricative component
        release = release of fricative component (seconds)
    */
    void setFricativeRelease(float release);
    /*
    setFricativeColour(float low, float high)

    Sets the cut off frequencies of the fricative bandpass filter
        low = low cut off frequency (Hz)
        high = high cut off frequency (Hz)
    */
    void setFricativeColour(float low, float high);

    /*
    getName()

    Returns the phoneme's name
    */
    std::string getName();
    /*
    getFofGain()

    Returns the gain of the FOF model output
    */
    float getFofGain();
    /*
    getBpGain()

    Returns the gain of the bandpass model outptu
    */
    float getBpGain();
    /*
    getFrequency(int formant)

    Returns the frequency of a specific formant (Hz)
        formant = formant number (0 - 4)
    */
    float getFrequency(int formant);
    /*
    getBandwidth(int formant)

    Returns the bandwidth of a specific formant (Hz)
        formant = formant number (0 - 4)
    */
    float getBandwidth(int formant);
    /*
    getGain(int formant)

    Returns the gain of a specific formant (0.0f - 1.0f)
        formant = formant number (0 - 4)
    */
    float getGain(int formant);
    /*
    getFormant(int formant)

    Returns the formant object associated with the given number
        formant = formant number (0 - 4)
    */
    Formant getFormant(int formant);
    /*
    getFricativeGain()
    
    Returns the gain of the fricative component
    */
    float getFricativeGain();
    /*
    getFricativeAttack()

    Returns the attack of the fricative component (seconds)
    */
    float getFricativeAttack();
    /*
    getFricativeDecay()

    Returns the decay of the fricative component (seconds)
    */
    float getFricativeDecay();
    /*
    getFricativeSustain()

    Returns the sustain of the fricative component (0.0f - 1.0f)
    */
    float getFricativeSustain();
    /*
    getFricativeRelease()

    Returns the release of the fricative component (seconds)
    */
    float getFricativeRelease();
    /*
    getFricativeLow()

    Returns the low cut off of the fricative bandpass (Hz)
    */
    float getFricativeLow();
    /*
    getFricativeHigh()

    Returns the high cut off of the fricative bandpass (Hz)
    */
    float getFricativeHigh();
private:
    std::string _name;
    Formant _formants[5];
    float _fofGain = 1.0f;
    float _bpGain = 0.5f;
    float _fricativeGain = 0.0f;
    float _fricativeAttack = 0.01f;
    float _fricativeDecay = 0.5f;
    float _fricativeSustain = 0.0f;
    float _fricativeRelease = 1.0f;
    float _fricativeLow = 20.0f;
    float _fricativeHigh = 20000.0f;
};