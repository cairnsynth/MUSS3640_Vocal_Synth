/*
  ==============================================================================

    Phoneme.cpp
    Created: 10 Apr 2023 5:34:13pm
    Author:  Luke Coles

  ==============================================================================
*/

#include "Phoneme.h"

void Phoneme::setName(std::string name)
{
    _name = name;
}

std::string Phoneme::getName()
{
    return _name;
}

void Phoneme::setFormant(int formant, float frequency, float bandwidth, float gain)
{
    setFrequency(formant, frequency);
    setBandwidth(formant, bandwidth);
    setGain(formant, gain);
}

void Phoneme::setFrequency(int formant, float frequency)
{
    if(20.0f < frequency < 20000.0f) { _formants[formant].frequency = frequency; }
    else if(frequency < 20.0f) { _formants[formant].frequency = 20.0f; }
    else { _formants[formant].frequency = 20000.0f; }
}

float Phoneme::getFrequency(int formant)
{
    return _formants[formant].frequency;
}

void Phoneme::setBandwidth(int formant, float bandwidth)
{
    if(0 < bandwidth < (2.0f * getFrequency(formant)))
    {
        _formants[formant].bandwidth = bandwidth;
    }
    else if (bandwidth > (2.0f * getFrequency(formant)))
    {
        _formants[formant].bandwidth = 2.0f * getFrequency(formant);
    }
    else { _formants[formant].bandwidth = 0.0f; }
}

float Phoneme::getBandwidth(int formant)
{
    return _formants[formant].bandwidth;
}

void Phoneme::setGain(int formant, float gain)
{
    if(0.0f <= gain <= 1.0f) { _formants[formant].gain = gain; }
    else if(1.0f < gain) { _formants[formant].gain = 1.0; }
    else if(gain < 0.0f) { _formants[formant].gain = 0.0f; }
}

float Phoneme::getGain(int formant) 
{
    return _formants[formant].gain;
}

Phoneme::Formant Phoneme::getFormant(int formant)
{
    return _formants[formant];
}

void Phoneme::setFricativeGain(float gain)
{
    if (0.0f < gain < 1.0f) { _fricativeGain = gain; }
    else if (gain > 1.0f) { _fricativeGain = 1.0; }
    else { _fricativeGain = 0.0f; }
}

float Phoneme::getFricativeGain()
{
    return _fricativeGain;
}

void Phoneme::setFricativeAttack(float attack)
{
    _fricativeAttack = attack;
}

float Phoneme::getFricativeAttack()
{
    return _fricativeAttack;
}

void Phoneme::setFricativeDecay(float decay)
{
    _fricativeDecay = decay;
}

float Phoneme::getFricativeDecay()
{
    return _fricativeDecay;
}

void Phoneme::setFricativeSustain(float sustain)
{
    _fricativeSustain = sustain;
}

float Phoneme::getFricativeSustain()
{
    return _fricativeSustain;
}

void Phoneme::setFricativeRelease(float release)
{
    _fricativeRelease = release;
}

float Phoneme::getFricativeRelease()
{
    return _fricativeRelease;
}

void Phoneme::setFricativeColour(float low, float high)
{
    if (low > 20.0f) { _fricativeLow = low; }
    else { _fricativeLow = 20.0f; }
    if (high < 20000.0f) { _fricativeHigh = high; }
    else { _fricativeHigh = 20000.0f; }
}

float Phoneme::getFricativeLow()
{
    return _fricativeLow;
}

float Phoneme::getFricativeHigh()
{
    return _fricativeHigh;
}