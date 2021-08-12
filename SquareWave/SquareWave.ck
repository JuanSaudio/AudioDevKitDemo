/* Define the number of oscillators */
10 => int numOsc;
220 => float fc;
SinOsc osc[numOsc];
for (0 => int i; i < numOsc; i++) {
    osc[i] => dac;
    osc[i].freq(fc * (i + 1));
    osc[i].gain(1.0 / (i + 1.0));
}
1::second => now;
