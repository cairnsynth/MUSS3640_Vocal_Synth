import("stdfaust.lib");
freq = hslider("Frequency", 110, 20, 1000, 1);

alpha0Slide = hslider("a_0", 0.2, 0.01, 2.0, 0.01);
alpha1Slide = hslider("a_1", 0.19, 0.01, 2.0, 0.01);
avSlide = hslider("Av", 1.0, 0.0, 2.0, 0.1);
t0Slide = hslider("T0", 1.0, 0.01, 2.0, 0.001);
teSlide = hslider("Te", 1.2, 0.00, 4.0, 0.001);

Av = avSlide;           //Amplitude of voicing
T0 = t0Slide;          //Glottal time interval
Tp = alpha0*T0;         //Glottal opening time
Tc = alpha1*T0;         //Glottal closing time
Te = Tc + Tp;               //Glottal closed time

alpha0 = alpha0Slide;          //Increasing slope
alpha1 = alpha1Slide;          //Decreasing slope

phasor(freq) = (+(freq/ma.SR) ~ ma.frac);

sinewave = sin(phasor(freq)*2*ma.PI);

/*
* REF:
* Stochastic models of glottal pulses from the Rosenberg and
*   Liljencrants-Fant models with unified parameters
* 
* E. Cataldo, D. Bahiano
* 
* <https://www.sciencedirect.com/science/article/pii/S0885230821000322> (pg 3)
*/
gPulse(t) = (-gPulse1_(t) - gPulse2_(t)) * part3_(t)
with {
    gPulse1_(t) = 0.5*Av*(1-cos((ma.PI*t)/Tp)) * part1_(t);
    gPulse2_(t) = Av*cos(((ma.PI*(t-Tp))/(2*Tc))) * part2_(t);
    part1_(t) = 0 < t < Tp;
    part2_(t) = Tp < t < Te;
    part3_(t) = (Te < t < T0);
};

process = gPulse(phasor(freq));

