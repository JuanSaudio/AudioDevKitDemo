clear
% Load a plugin into matlab
path = "/Users/juans/Developer/MeyerSound/AudioDevKit/Distortion/Builds/MacOSX/build/Debug/Distortion.vst3";
plugin = loadAudioPlugin(path);
%%

% Define plugin settings
plugin.Bias = 0;
plugin.Drive = 10;
plugin.Level = -10;

% Create a signal
N = 2^12;
n = (0:N-1)';
% with 50 periods
fc = 50;
x = sin(2 * pi * fc * n / N);
% allocate space for output
y = zeros(size(x));

% process per block
ni = 0;
Ni = 1024;

% process
while(ni < N)
    xi = x(ni+1:ni+Ni);
    yi = plugin.process(xi);
    y(ni+1:ni+Ni) = yi;
    ni = ni + Ni;
end

% Remove DC
x = x - mean(x);
y = y - mean(y);
% Normalize Amplitude
x = x / max(abs(x));
y = y / max(abs(y));

% Get Spectra
X = fft(x) / N;
Y = fft(y) / N;
X = X(1:end/2);
Y = Y(1:end/2);
f = (0:N/2-1) / N;

% Set Amplitude Threshold
th = -60;

subplot 311
plot(n/N, y)
hold on
plot(n/N, x)
hold off
grid on
xlabel("Normalized Time")
ylabel("Amplitude")
title("Waveform")

subplot 312
plot(f, db(Y))
hold on
plot(f, db(X))
hold off
grid on
xlim([0, 0.5])
ylim([th, 0])
xlabel("Normalized Frequency")
ylabel("Amplitude")
title("Magnitude Spectrum")

subplot 313
plot(f, deg(Y) .* (db(Y) > th))
hold on
plot(f, deg(X) .* (db(X) > th))
hold off
grid on
xlim([0, 0.5])
ylim([-180, 180])
xlabel("Normalized Frequency")
ylabel("Amplitude")
title("Phase Spectrum")
