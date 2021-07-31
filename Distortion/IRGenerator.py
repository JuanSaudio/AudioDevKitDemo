#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jul 30 08:45:45 2021

@author: juans
"""

import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sg
import soundfile as sf

def db(x):
    return 20. * np.log10(np.abs(x))

def deg(x):
    return np.rad2deg(np.angle(x))

def rms(x):
    return np.sqrt(np.mean(np.abs(x)**2.0))


x, fs = sf.read("ReferenceGuitarCabinet.wav")
y, fs = sf.read("MeasurementGuitarCabinet.wav")


# Cross Correlation
r = sg.correlate(x, y, 'full')
delay = np.size(x) - np.argmax(r)
delay += 9 # Manual Adjustment
y = y * 2
y = np.roll(y, -delay + 1)

# STFT Settings
nFFT = 2**16
olap = 2**15
win = sg.windows.blackmanharris(nFFT)

# STFT
f, _, X = sg.stft(x, fs, win, nFFT, olap, nFFT, return_onesided=False)
f, _, Y = sg.stft(y, fs, win, nFFT, olap, nFFT, return_onesided=False)

# Auto and cross spectrum
Sxx = np.mean(np.abs(X)**2, 1)
Syy = np.mean(np.abs(Y)**2, 1)
Sxy = np.mean(np.conj(X) * Y, 1)

# Frequency Response Estimations
H1 = Sxy / Sxx;
H2 = Syy / np.conj(Sxy);

# Coherence
c = (np.abs(Sxy)**2) / (Sxx * Syy);

# Impulse Response
h = np.real(np.fft.ifft(H1))
h = np.roll(h, nFFT//2)
t = np.arange(-nFFT/2, nFFT/2) / fs

f = f[:nFFT//2]
c = c[:nFFT//2]
H1Mag = db(H1[:nFFT//2])
H1Ang = deg(H1[:nFFT//2])
H2Mag = db(H2[:nFFT//2])
H2Ang = deg(H2[:nFFT//2])

# Coherence Blanking
th = 0.8

H1Mag[c < th] = np.nan
H1Ang[c < th] = np.nan
H2Mag[c < th] = np.nan
H2Ang[c < th] = np.nan

# Extra Padding
startIdx = np.nonzero(np.abs(h) > 2.0 * rms(h))[0][0]
endIdx = np.nonzero(np.abs(h) > 2.0 * rms(h))[0][-1]
startIdx -= (nFFT//2 - startIdx)//2
endIdx += 2 * (endIdx - nFFT//2)

plt.clf()
plt.subplot(3, 1, 1)
plt.plot(t, h)
plt.vlines(t[startIdx], -1, 1, colors='r', linestyles=':')
plt.vlines(t[endIdx], -1, 1, colors='r', linestyles=':')
plt.grid(True)
plt.xlim(t[[0, -1]])
plt.ylim(np.max(np.abs(h)) * np.array([-1, 1]))
plt.xlabel("Time")
plt.ylabel("Amplitude")
plt.title("Impulse Response")

plt.subplot(3, 1, 2)
plt.semilogx(f, H1Mag);
plt.xlim([2e1, 2e4])
plt.ylim([-18, 12])
plt.yticks(np.arange(-18,12+1,6))
plt.grid(True)
plt.xlabel("Frequency")
plt.ylabel("Amplitude")
plt.title("Amplitude Response")

plt.subplot(3, 1, 3)
plt.semilogx(f, H1Ang)
plt.xlim([2e1, 2e4])
plt.ylim([-180, 180])
plt.yticks(np.arange(-180,180+1,60))
plt.grid(True)
plt.xlabel("Frequency")
plt.ylabel("Phase")
plt.title("Phase Response")
plt.tight_layout()

h = h[startIdx:endIdx]
win = sg.windows.tukey(np.size(h), 0.99)
h *= win

sf.write("CabinetIR.wav", h, fs, 'PCM_24')
