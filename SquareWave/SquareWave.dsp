import("stdfaust.lib");
nOsc = 10; // number of oscillators
fc = 100; // base frequency
process = par(i, nOsc, os.osc(fc * (i + 1)) / (i + 1)) :> _ * 0.00001;
