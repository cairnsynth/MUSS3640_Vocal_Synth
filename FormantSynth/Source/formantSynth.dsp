import("stdfaust.lib");

gate = button("gate") : ba.toggle;
inFreq = hslider("freq", 110, 20, 1000, 1);
vibratoFreq = nentry("vibratoFreq", 2.0, 0.0, 10.0, 0.1);
vibratoGain = nentry("vibratoGain", 0.2, 0.0, 1.0, 0.01);
noiseGain = nentry("noiseGain", 0.8, 0.0, 1.0, 0.01);
noiseColour = hslider("noiseColour", 10000.0, 6000.0, 15000.0, 1.0);
vocalFry = hslider("vocalFry", 0.1, 0.1, 1.0, 0.01);



f1Freq = nentry("f1Freq", 800, 20, 20000, 1);
f2Freq = nentry("f2Freq", 1150, 20, 20000, 1);
f3Freq = nentry("f3Freq", 2800, 20, 20000, 1);
f4Freq = nentry("f4Freq", 3500, 20, 20000, 1);
f5Freq = nentry("f5Freq", 4950, 20, 20000, 1);

f1Gain = nentry("f1Gain", 1, 0, 1, 0.001);
f2Gain = nentry("f2Gain", 0.63, 0, 1, 0.001);
f3Gain = nentry("f3Gain", 0.1, 0, 1, 0.001);
f4Gain = nentry("f4Gain", 0.015, 0, 1, 0.001);
f5Gain = nentry("f5Gain", 0.001, 0, 1, 0.001);

f1BW = nentry("f1BW", 80, 20, 1000, 1);
f2BW = nentry("f2BW", 90, 20, 1000, 1);
f3BW = nentry("f3BW", 120, 20, 1000, 1);
f4BW = nentry("f4BW", 130, 20, 1000, 1);
f5BW = nentry("f5BW", 140, 20, 1000, 1);


formant(freq, width, gain) = *(gain_) : fi.bandpass(1, freqLow_, freqHigh_)
with {
    freqLow_ = freq - (width/2) : si.smoo;
    freqHigh_ = freq + (width/2) : si.smoo;
    gain_ = gain : si.smoo;
};

f1 = formant(f1Freq, f1BW, f1Gain);
f2 = formant(f2Freq, f2BW, f2Gain);
f3 = formant(f3Freq, f3BW, f3Gain);
f4 = formant(f4Freq, f4BW, f4Gain);
f5 = formant(f5Freq, f5BW, f5Gain);

voiceEnv = en.adsr(0.2, 1.0, 0.6, 1.0, gate);
noiseEnv = en.adsr(0.01, 0.3, 0.0, 0.0, gate);

freq = inFreq + (os.sinwaveform(vibratoFreq)*vibratoGain);
vibrato = os.osc(vibratoFreq)*vibratoGain;
voiced = ((os.square(freq+vibrato)*(1.0-vocalFry)) : +((os.square(freq+vibrato) : ba.impulsify)*vocalFry*3.0))*voiceEnv;
noise = no.noise*noiseGain : fi.bandpass(1, noiseColour - 5000, noiseColour + 5000);
source = voiced : +(noise*noiseEnv);

process = source <: f1, f2, f3, f4, f5:> _ <: _,_;