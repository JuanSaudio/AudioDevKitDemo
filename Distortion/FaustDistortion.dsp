// Faust Example of simple distortion
import("stdfaust.lib");
// Define Sliders
bias = hslider("Bias",0,0,1,0.001);
drive = hslider("Drive",0,-20,20,0.1) : ba.db2linear;
outGain = hslider("Level",0,-20,20,0.1) : ba.db2linear;

// Define Non Linearity
nonLinearity = atan((drive * _ + bias) * ma.PI / 2.0) * 2.0 / ma.PI * outGain;

// Process removing DC
process = _ : nonLinearity : fi.dcblocker;

// faust FaustDistortion.dsp -o Source/FaustDistortion.h -cn Distortion -ns faust
