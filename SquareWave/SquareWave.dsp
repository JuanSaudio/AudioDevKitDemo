import("stdfaust.lib");
nOsc = 10;
fc = 100;
process = par(i, nOsc, os.osc(fc * (i + 1)) / (i + 1)) :> _;
