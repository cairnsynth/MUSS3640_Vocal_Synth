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
    _formants[formant].frequency = frequency;
    _formants[formant].bandwidth = bandwidth;
    _formants[formant].gain = gain;
}

void Phoneme::setFrequency(int formant, float frequency)
{
    _formants[formant].frequency = frequency;
}

float Phoneme::getFrequency(int formant)
{
    return _formants[formant].frequency;
}

void Phoneme::setBandwidth(int formant, float bandwidth)
{
    _formants[formant].bandwidth = bandwidth;
}

float Phoneme::getBandwidth(int formant)
{
    return _formants[formant].bandwidth;
}

void Phoneme::setGain(int formant, float gain)
{
    _formants[formant].gain = gain;
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
    _fricativeGain = gain;
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
    _fricativeLow = low;
    _fricativeHigh = high;
}

float Phoneme::getFricativeLow()
{
    return _fricativeLow;
}

float Phoneme::getFricativeHigh()
{
    return _fricativeHigh;
}