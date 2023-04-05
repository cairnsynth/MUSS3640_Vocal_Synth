declare options "[nvoices:8]";

import("stdfaust.lib");

/*INPUT/API*/
gate = button("gate");

//Formant Data
f1Freq = nentry("f1Freq", 800, 20, 20000, 1);
f1Gain = nentry("f1Gain", 1.0, 0.0, 1.0, 0.01);
f1BW = nentry("f1BW", 80, 20, 20000, 1);

f2Freq = nentry("f2Freq", 1150, 20, 20000, 1);
f2Gain = nentry("f2Gain", 0.630957, 0.0, 1.0, 0.0001);
f2BW = nentry("f2BW", 90, 20, 20000, 1);

f3Freq = nentry("f3Freq", 2800, 20, 20000, 1);
f3Gain = nentry("f3Gain", 0.100000, 0.0, 1.0, 0.0001);
f3BW = nentry("f3BW", 120, 20, 20000, 1);

f4Freq = nentry("f4Freq", 3500, 20, 20000, 1);
f4Gain = nentry("f4Gain", 0.015849, 0.0, 1.0, 0.0001);
f4BW = nentry("f4BW", 130, 20, 20000, 1);

f5Freq = nentry("f5Freq", 4950, 20, 20000, 1);
f5Gain = nentry("f5Gain", 0.001000, 0.0, 1.0, 0.0001);
f5BW = nentry("f5BW", 140, 20, 20000, 1);

//Voice Envelope Control
voiceAttack = vslider("t:voice/h:envelope/voiceAttack", 0.01, 0.01, 2.0, 0.01);
voiceDecay = vslider("t:voice/h:envelope/voiceDecay", 0.01, 0.01, 2.0, 0.01);
voiceSustain = vslider("t:voice/h:envelope/voiceSustain", 1.0, 0.0, 1.0, 0.01);
voiceRelease = vslider("t:voice/h:envelope/voiceRelease", 1.0, 0.01, 5.0, 0.01);

//Source Control
bpSourcePW = vslider("t:voice/h:source/bpSourcePW", 0.1, 0.001, 0.999, 0.001);
bpSourceSelect = vslider("t:voice/h:source/bpSourceSelect", 0, 0, 2, 1);
glideTime = vslider("t:voice/h:source/glideTime", 0.0, 0.0, 2.0, 0.001);
frequency = vslider("freq", 100, 20, 1046, 0.1) : si.smooth(ba.tau2pole(glideTime));
noiseSlide = vslider("t:voice/h:source/noise", 0.001, 0.0, 1.0, 0.001);
t0Slide = vslider("t:voice/h:source/T0", 0.915, 0.01, 1.0, 0.001);
teSlide = vslider("t:voice/h:source/Te", 0.857, 0.00, 4.0, 0.001);
pressureSlide = vslider("t:voice/h:source/Pressure", 1.0, 0.0, 1.0, 0.001);

//Fricative Control
noiseColourLow = vslider("t:voice/h:fricative/noiseColourLow", 20, 20, 20000, 1);
noiseColourHigh = vslider("t:voice/h:fricative/noiseColourHigh", 20000, 20, 20000, 1);

noiseAttack = vslider("t:voice/h:fricative/noiseAttack", 0.01, 0.01, 2.0, 0.01);
noiseDecay = vslider("t:voice/h:fricative/noiseDecay", 0.01, 0.01, 2.0, 0.01);
noiseSustain = vslider("t:voice/h:fricative/noiseSustain", 0.0, 0.0, 1.0, 0.001);
noiseRelease = vslider("t:voice/h:fricative/noiseRelease", 0.01, 0.01, 3.0, 0.01);


//Vibrato Control
vibratoGain = vslider("t:voice/h:vibrato/vibratoGain", 0.3, 0.0, 1.0, 0.01);
vibratoFreq = vslider("t:voice/h:vibrato/vibratoFreq", 5.0, 0.01, 8.0, 0.01);
vibratoAttack = vslider("t:voice/h:vibrato/vibratoAttack", 0.01, 0.01, 2.0, 0.01);
vibratoSustain = vslider("t:voice/h:vibrato/vibratoSustain", 1.0, 0.0, 1.0, 0.01);
vibratoRelease = vslider("t:voice/h:vibrato/vibratoRelease", 0.01, 0.01, 5.0, 0.01);

//Gain Control
fofGain = vslider("t:voice/h:mixer/fofGain", 0.5, 0.0, 1.0, 0.01);
bpGain = vslider("t:voice/h:mixer/bpGain", 0.5, 0.0, 1.0, 0.01);
fricativeGain = vslider("t:voice/h:mixer/fricativeGain", 0.5, 0.0, 1.0, 0.01);
gain = vslider("gain", 0.5, 0.0, 1.0, 0.01);

//unisonDetune = hslider("unisonDetune", 0.0, 0.0, 2.0, 0.01);

//Global signals
nFormants = 5;
nUnison = 4;
unisonDetune = 0;
voiceEffort = pm.vocalEffort(sourceFreq, gender);
index = (voiceType * nFormants) + vowel : si.smoo;
sourceFreq = frequency + vibratoSignal;

/*ENVLOPE*/
voiceEnvelope = en.adsr(voiceAttack, voiceDecay, voiceSustain, voiceRelease, gate);

/*VIBRATO*/
vibratoSignal = os.osc(vibratoFreq)*vibratoEnvelope;
vibratoEnvelope = en.asr(vibratoAttack, vibratoSustain, vibratoRelease, gate);

/*FOF*/
fofSource(freq_) = os.lf_imptrain(freq_);

//FOF Formant Object
fofFormant(freq_, bw_, gain_) = pm.fofCycle(_freq, _bw, _sw, _gain, 6)
with {
    _freq = freq_;
    _bw = bw_ : si.smooth(ba.tau2pole(0.001));
    _sw = bw_ : si.smooth(ba.tau2pole(0.001));
    _gain = gain_;
};

//FOF Formant Bank

fofBank = _fof1, _fof2, _fof3, _fof4, _fof5 :> _ <: _,_
with {
    _fof1 = fofFormant(f1Freq, f1BW, f1Gain);
    _fof2 = fofFormant(f2Freq, f2BW, f2Gain);
    _fof3 = fofFormant(f3Freq, f3BW, f3Gain);
    _fof4 = fofFormant(f4Freq, f4BW, f4Gain);
    _fof5 = fofFormant(f5Freq, f5BW, f5Gain);
};

//FOF Gain Compensation
fofGainComp = 50;

//FOF Process Block
fofChain = par(i, nUnison,(fofSource(sourceFreq + ((unisonDetune/nUnison) * i)) <: fofBank : *((1/nUnison)*i), *(1-((1/nUnison)*i)))) :> co.compressor_stereo(5, -30, 0.05, 0.5): *(fofGainComp),*(fofGainComp) : *(voiceEnvelope), *(voiceEnvelope) : *(fofGain),*(fofGain);

/*BANDPASS*/
bpSource(freq_) = _square, _saw, _model : select3(bpSourceSelect) :> _
with {
    _square = os.pulsetrain(freq_, bpSourcePW);
    _saw = os.sawtooth(freq_);
    _model = rosenbergModel(freq_, 1.0, t0Slide, teSlide, 0.441, 0.224);
};

bpFormant(freq_, bw_, gain_) = fi.bandpass(1, _freqLow, _freqHigh) : *(_gain)
with {
    _freqLow = freq_ - (bw_/2);
    _freqHigh = freq_ + (bw_/2);
    _gain = gain_;
};

//Bandpass formant bank
bpBank = _ <: _bp1, _bp2, _bp3, _bp4, _bp5 :> _ <: _,_
with {
    _bp1 = bpFormant(f1Freq, f1BW, f1Gain);
    _bp2 = bpFormant(f2Freq, f2BW, f2Gain);
    _bp3 = bpFormant(f3Freq, f3BW, f3Gain);
    _bp4 = bpFormant(f4Freq, f4BW, f4Gain);
    _bp5 = bpFormant(f5Freq, f5BW, f5Gain);
};

bpChain = par(i, nUnison,(bpSource(sourceFreq + ((unisonDetune/nUnison) * i)) <: bpBank : *((1/nUnison)*i), *(1-((1/nUnison)*i)))) :> co.compressor_stereo(5, -30, 0.05, 0.5) : *(10), *(10) : *(voiceEnvelope), *(voiceEnvelope) : *(bpGain), *(bpGain);

/*FRICATIVE*/
fricativeSource = no.noise : fi.bandpass(1, noiseColourLow, noiseColourHigh);
fricativeEnvelope = en.adsr(noiseAttack, noiseDecay, noiseSustain, noiseRelease, gate);

fricativeChain = fricativeSource : bpBank <: *(fricativeGain)*(fricativeEnvelope), *(fricativeGain)*(fricativeEnvelope);

/* Glottal Model */

phasor(freq) = (+(freq/ma.SR) ~ ma.frac);

rosenbergModel(freq, Av, T0, Te, a0, a1) = ba.if(rCond2_(t_), rosenberg1_(-t_), rosenberg2_(-t_)) * rCond3_(t_)
with {
    t_ = (phasor(freq)*pressureSlide) + (no.noise*noiseSlide : fi.highpass(1, 22000));
    Tp_ = a0*T0;
    Tc_ = a1*T0;
    Te_ = Te;
    rosenberg1_(t_) = 0.5*Av*(1-cos(ma.PI*t_/Tp_)) * rCond1_(t_);
    rosenberg2_(t_) = Av*cos(((ma.PI*(t_-Tp_))/(2*Tc_))) * rCond2_(t_);
    rCond1_(t_) = 0 < t_ < Tp_;
    rCond2_(t_) = Tp_ < t_ < Te_;
    rCond3_(t_) = (Te_ < t_ < T0);
};

process = bpChain, fofChain, fricativeChain :> *(gain), *(gain) : _,_;