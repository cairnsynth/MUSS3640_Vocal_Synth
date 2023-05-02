import("stdfaust.lib");
inFreq = hslider("Frequency", 110, 20, 1000, 1);

alpha0Slide = hslider("a_0", 0.2, 0.01, 2.0, 0.001);
alpha1Slide = hslider("a_1", 0.19, 0.01, 2.0, 0.001);
avSlide = hslider("Av", 1.0, 0.0, 2.0, 0.1);
t0Slide = hslider("T0", 1.0, 0.01, 2.0, 0.001);
teSlide = hslider("Te", 1.2, 0.00, 4.0, 0.001);

jitterGainSlide = hslider("Jitter Gain", 0.0, 0.0, 0.1, 0.001);
jitterFreqSlide = hslider("Jitter Freq", 20, 1, 100, 1);

noiseSlide = hslider("Noise", 0.001, 0.0, 1.0, 0.001);
noiseColSlide = hslider("Noise Colour", 20000, 20, 24000, 1);

pressureSlide = hslider("Pressure", 1.0, 0.0, 1.0, 0.001);

inharmonicSlide = hslider("Inharmonics", 1.0, 0.0, 5.0, 0.001);
ihRatioSlide = hslider("Inharmonic Ratio", 1.0, 0.0, 5.0, 0.01);

jitter = os.osc(jitterFreqSlide)*jitterGainSlide;

Av = avSlide;           //Amplitude of voicing
T0 = t0Slide;          //Glottal time interval
//Tp = alpha0*T0;         //Glottal opening time
//Tc = alpha1*T0;         //Glottal closing time
//Te = Tc + Tp;               //Glottal closed time

alpha0 = alpha0Slide + jitter;          //Increasing slope
alpha1 = alpha1Slide + jitter;          //Decreasing slope

phasor(freq) = (+(freq/ma.SR) ~ ma.frac);

freq = inFreq + (os.osc(inFreq*ihRatioSlide)*inharmonicSlide) + (os.osc(inFreq*ihRatioSlide*1.5)*inharmonicSlide);

/*
* REF:
* Stochastic models of glottal pulses from the Rosenberg and
*   Liljencrants-Fant models with unified parameters
* 
* E. Cataldo, D. Bahiano
* 
* <https://www.sciencedirect.com/science/article/pii/S0885230821000322> (pg 3)
* 
* Params:
*   -Av = Amplitude of voicing
*   -T0 = Glottal time interval
*   -Te = Glottal closed time
*   -a0 = Increase slope constant
*   -a1 = Decrease slope constant
*/
rosenbergModel(freq, Av, T0, Te, a0, a1) = ba.if(rCond2_(t_), rosenberg1_(-t_), rosenberg2_(-t_)) * rCond3_(t_)
with {
    t_ = (phasor(freq)*pressureSlide) + (no.noise*noiseSlide : fi.highpass(1, noiseColSlide));
    Tp_ = a0*T0;
    Tc_ = a1*T0;
    Te_ = Te;
    rosenberg1_(t_) = 0.5*Av*(1-cos(ma.PI*t_/Tp_)) * rCond1_(t_);
    rosenberg2_(t_) = Av*cos(((ma.PI*(t_-Tp_))/(2*Tc_))) * rCond2_(t_);
    rCond1_(t_) = 0 < t_ < Tp_;
    rCond2_(t_) = Tp_ < t_ < Te_;
    rCond3_(t_) = (Te_ < t_ < T0);
};


process = rosenbergModel(freq, Av, T0, teSlide, alpha0, alpha1) <: _,_;

