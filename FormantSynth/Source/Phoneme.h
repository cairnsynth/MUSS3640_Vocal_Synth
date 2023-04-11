/*
  ==============================================================================

    Phoneme.h
    Created: 10 Apr 2023 5:34:13pm
    Author:  Luke Coles

  ==============================================================================
*/
#include <string>
#pragma once

class Phoneme {

public:
    struct Formant {
        float frequency;
        float bandwidth;
        float gain;
    };

    Phoneme() {}

    Phoneme(std::string name, Formant f1, Formant f2, Formant f3, Formant f4, Formant f5) {
        _name = name;
        _formants[0] = f1;
        _formants[1] = f2;
        _formants[2] = f3;
        _formants[3] = f4;
        _formants[4] = f5;
    }
    void setName(std::string name);
    void setFormant(int formant, float frequency, float bandwidth, float gain);
    void setFrequency(int formant, float frequency);
    void setBandwidth(int formant, float bandwidth);
    void setGain(int formant, float gain);
    void setFricativeGain(float gain);
    void setFricativeAttack(float attack);
    void setFricativeDecay(float decay);
    void setFricativeSustain(float sustain);
    void setFricativeRelease(float release);
    void setFricativeColour(float low, float high);

    std::string getName();
    float getFrequency(int formant);
    float getBandwidth(int formant);
    float getGain(int formant);
    Formant getFormant(int formant);
    float getFricativeGain();
    float getFricativeAttack();
    float getFricativeDecay();
    float getFricativeSustain();
    float getFricativeRelease();
    float getFricativeLow();
    float getFricativeHigh();
private:
    std::string _name;
    Formant _formants[5];
    float _fricativeGain = 0.0f;
    float _fricativeAttack = 0.01f;
    float _fricativeDecay = 0.5f;
    float _fricativeSustain = 0.0f;
    float _fricativeRelease = 1.0f;
    float _fricativeLow = 20.0f;
    float _fricativeHigh = 20000.0f;
};