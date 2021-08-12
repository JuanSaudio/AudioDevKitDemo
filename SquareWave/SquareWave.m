clear
% Define a sample rate
fs = 48000;
fc = 220;
nOsc = 10;
dur = 1;
t = linspace(0, dur, dur * fs + 1);
f = fc * (1:nOsc);
g = 1 ./ (1:nOsc);
x = g * sin(2 * pi * f' * t);
soundsc(x, fs);
