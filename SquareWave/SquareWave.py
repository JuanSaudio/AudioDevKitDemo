import numpy as np
import sounddevice as sd

fs = 48000
fc = 100
nOsc = 10
dur = 1
t = np.linspace(0, dur, dur * fs + 1)
f = fc * np.arange(1,nOsc + 1)
g = 1.0 / np.arange(1, nOsc + 1)
x = np.dot(g, np.sin(2.0 * np.pi * np.outer(f, t)))
sd.play(x, fs)
sd.wait()
