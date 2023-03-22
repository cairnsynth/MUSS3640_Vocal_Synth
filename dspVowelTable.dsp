import("stdfaust.lib");

//https://github.com/grame-cncm/faustlibraries/blob/master/physmodels.lib
values = environment {
    f(0) = (800,400,350,450,325,600,400,250,400,350,660,440,270,430,370,800,
		350,270,450,325,650,400,290,400,350); // formant 0 freqs
	f(1) = (1150,1600,1700,800,700,1040,1620,1750,750,600,1120,1800,1850,820,630,
		1150,2000,2140,800,700,1080,1700,1870,800,600); // formant 1 freqs
	f(2) = (2800,2700,2700,2830,2530,2250,2400,2600,2400,2400,2750,2700,2900,2700,2750,
		2900,2800,2950,2830,2700,2650,2600,2800,2600,2700); // formant 2 freqs
	f(3) = (3500,3300,3700,3500,3500,2450,2800,3050,2600,2675,3000,3000,3350,3000,3000,
		3900,3600,3900,3800,3800,2900,3200,3250,2800,2900); // formant 3 freqs
	f(4) = (4950,4950,4950,4950,4950,2750,3100,3340,2900,2950,3350,3300,3590,3300,3400,
		4950,4950,4950,4950,4950,3250,3580,3540,3000,3300); // formant 4 freqs
	g(0) = (1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1); // formant 0 gains
	g(1) = (0.630957,0.063096,0.100000,0.354813,0.251189,0.446684,0.251189,0.031623,
		0.281838,0.100000,0.501187,0.199526,0.063096,0.316228,0.100000,
		0.501187,0.100000,0.251189,0.281838,0.158489,0.501187,0.199526,0.177828,
		0.316228,0.100000); // formant 1 gains
	g(2) = (0.100000,0.031623,0.031623,0.158489,0.031623,0.354813,0.354813,0.158489,
		0.089125,0.025119,0.070795,0.125893,0.063096,0.050119,0.070795,
		0.025119,0.177828,0.050119,0.079433,0.017783,0.446684,0.251189,0.125893,
		0.251189,0.141254); // formant 2 gains
	g(3) = (0.015849,0.017783,0.015849,0.039811,0.010000,0.354813,0.251189,0.079433,
		0.100000,0.039811,0.063096,0.100000,0.015849,0.079433,0.031623,
		0.100000,0.010000,0.050119,0.079433,0.010000,0.398107,0.199526,0.100000,
		0.251189,0.199526); // formant 3 gains
	g(4) = (0.001000,0.001000,0.001000,0.001778,0.000631,0.100000,0.125893,0.039811,
		0.010000,0.015849,0.012589,0.100000,0.015849,0.019953,0.019953,
		0.003162,0.001585,0.006310,0.003162,0.001000,0.079433,0.100000,0.031623,
		0.050119,0.050119); // formant 4 gains
	bw(0) = (80,60,50,70,50,60,40,60,40,40,80,70,40,40,40,80,60,60,40,50,
    50,70,40,70,40); // formant 0 bandwidths
	bw(1) = (90,80,100,80,60,70,80,90,80,80,90,80,90,80,60,90,100,90,80,60,
		90,80,90,80,60); // formant 1 bandwidths
	bw(2) = (120,120,120,100,170,110,100,100,100,100,120,100,100,100,100,
		120,120,100,100,170,120,100,100,100,100); // formant 2 bandwidths
	bw(3) = (130,150,150,130,180,120,120,120,120,120,130,120,120,120,120,
		130,150,120,120,180,130,120,120,130,120); // formant 3 bandwidths
	bw(4) = (140,200,200,135,200,130,120,120,120,120,140,120,120,120,120,
		140,200,120,120,200,140,120,120,135,120); // formant 4 bandwidths
};


/*INPUT/API*/
gate = button("gate") : ba.toggle;

voiceType = hslider("voiceType", 0, 0, 4, 1);
vowel = hslider("vowel", 0, 0, 4, 0.01) : si.smoo;
gender = hslider("gender", 0, 0, 1, 1) : si.smoo;
frequency = hslider("frequency", 100, 20, 1046, 0.1) : si.smoo;
sourcePW = hslider("sourcePW", 0.1, 0.001, 0.999, 0.001);

unisonDetune = hslider("unisonDetune", 0.0, 0.0, 2.0, 0.01);
//Vibrato Control
vibratoGain = hslider("vibratoGain", 0.3, 0.0, 1.0, 0.01);
vibratoFreq = hslider("vibratoFreq", 5.0, 0.01, 8.0, 0.01);
vibratoAttack = hslider("vibratoAttack", 0.01, 0.01, 2.0, 0.01);
vibratoSustain = hslider("vibratoSustain", 1.0, 0.0, 1.0, 0.01);
vibratoRelease = hslider("vibratoRelease", 0.01, 0.01, 5.0, 0.01);

//Gain Control
fofGain = hslider("fofGain", 0.5, 0.0, 1.0, 0.01);
bpGain = hslider("bpGain", 0.5, 0.0, 1.0, 0.01);
gain = hslider("gain", 0.5, 0.0, 1.0, 0.01);

//Global signals
nFormants = 5;
nUnison = 3;
voiceEffort = pm.vocalEffort(sourceFreq, gender);
index = (voiceType * nFormants) + vowel : si.smoo;
sourceFreq = frequency + vibratoSignal;

getFormantFreq(formantNumber_) = ba.listInterp(values.f(formantNumber_), index); // : pm.autobendFreq(0, sourceFreq, voiceType);
getFormantGain(formantNumber_) = ba.listInterp(values.g(formantNumber_), index) : voiceEffort;
getFormantBW(formantNumber_) = ba.listInterp(values.bw(formantNumber_), index);
getFormantSW(formantNumber_) = pm.skirtWidthMultiplier(vowel, sourceFreq, gender) * getFormantBW(formantNumber_);

/*VIBRATO*/
vibratoSignal = os.osc(vibratoFreq)*vibratoEnvelope;
vibratoEnvelope = en.asr(vibratoAttack, vibratoSustain, vibratoRelease, gate);

/*FRICATIVE*/

/*FOF*/
//Source Impulse Train
fofSource(freq_) = os.lf_imptrain(freq_);

//FOF Formant Object
fofFormant(formantNumber_) = pm.fofCycle(_freq, _bw, _sw, _gain, 6)
with {
    _freq = getFormantFreq(formantNumber_);
    _bw = getFormantBW(formantNumber_) : si.smooth(ba.tau2pole(0.001));
    _sw = getFormantSW(formantNumber_) : si.smooth(ba.tau2pole(0.001));
    _gain = getFormantGain(formantNumber_);
};

//FOF Formant Bank
fofBank = par(i, nFormants, fofFormant(i)) :> _ <: _,_;

//FOF Gain Compensation
fofAmplitude = fofBank :> abs : an.amp_follower(0.2) : ba.db2linear;
fofGainComp = 30;
//fofGainComp = 100;

//FOF Process Block
fofChain = par(i, nUnison,(fofSource(sourceFreq + ((unisonDetune/nUnison) * i)) <: fofBank : *((1/nUnison)*i), *(1-((1/nUnison)*i)))) :> _ <: co.compressor_stereo(5, -30, 0.05, 0.5): *(fofGainComp),*(fofGainComp) : *(fofGain),*(fofGain);

//fofAmplitude = fofBank : abs : an.amp_follower(0.5);
//fofGainComp = 1.0 / fofAmplitude;

/*BANDPASS*/
bpSource = os.pulsetrain(sourceFreq, sourcePW) : fi.bandstop(1, 1000, 1500);

bpFormant(formantNumber_) = fi.bandpass(1, _freqLow, _freqHigh) : *(_gain)
with {
    _freqLow = getFormantFreq(formantNumber_) - (getFormantBW(formantNumber_)/2);
    _freqHigh = getFormantFreq(formantNumber_) + (getFormantBW(formantNumber_)/2);
    _gain = getFormantGain(formantNumber_);
};

//Bandpass formant bank
bpBank = _ <: _bp0, _bp1, _bp2, _bp3, _bp4 :> _
with {
    _bp0 = bpFormant(0);
    _bp1 = bpFormant(1);
    _bp2 = bpFormant(2);
    _bp3 = bpFormant(3);
    _bp4 = bpFormant(4);
};

envelope = abs : max ~ -(1.0/ma.SR) : max(ba.db2linear(-70)) : ba.linear2db;
//process = fofSource : fofBank*fofGain :> *(gain) <: _,_;
process = fofChain : *(gain), *(gain) : _,_;

