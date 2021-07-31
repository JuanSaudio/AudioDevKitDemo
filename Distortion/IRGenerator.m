clear

[x, ~ ] = audioread("ReferenceGuitarCabinet.wav");
[y, fs] = audioread("MeasurementGuitarCabinet.wav");

% Get Cross Correlation Delay
[r, lags] = xcorr(x, y);
[~, idx] = max(abs(r));
delay = lags(idx);
delay = delay + 9; % Manual Adjustment
y = y * db2mag(9); % Manual Gain
y = circshift(y, delay);

% STFT Settings
nFFT = 2^16;
olap = 2^15;
win = blackmanharris(nFFT);

% STFT
[X, ~] = stft(x, fs, "Window", win, "OverlapLength", olap, "FFTLength", nFFT, "Centered", false);
[Y, f] = stft(y, fs, "Window", win, "OverlapLength", olap, "FFTLength", nFFT, "Centered", false);

% AutoSpectra and XSpectra
Sxx = mean(abs(X).^2, 2);
Syy = mean(abs(Y).^2, 2);
Sxy = mean(conj(X) .* Y, 2);

% Estimations of frequency Response
H1 = Sxy ./ Sxx;
H2 = Syy ./ conj(Sxy);

% Coherence
c = (abs(Sxy).^2) ./ (Sxx .* Syy);

% Impulse response
h = ifft(H1);
% Make sure it is real
h = real(h);
% Shift to the center
h = circshift(h, nFFT/2);


t = (-nFFT/2:nFFT/2-1) / fs;
f = f(1:end/2);
c = c(1:end/2);
H1Mag = db(H1(1:end/2));
H1Ang = deg(H1(1:end/2));
H2Mag = db(H2(1:end/2));
H2Ang = deg(H2(1:end/2));

% Coherence Blanking
th = 0.8;
H1Mag(c < th) = nan;
H2Mag(c < th) = nan;
H1Ang(c < th) = nan;
H2Ang(c < th) = nan;

% Add a bit 
h = h / max(abs(h));
startIdx = find(abs(h) > 2 * rms(h), 1);
endIdx = find(abs(h) > 2 * rms(h), 1, "last");
startIdx = floor(startIdx - (nFFT/2 - startIdx)/2);
endIdx = ceil(endIdx + 2 * (endIdx - nFFT/2));


clf()
subplot 311
plot(t, h)
vline(t(startIdx), "LineWidth", 0.5, "LineStyle", ":", "Color", "r")
vline(t(endIdx), "LineWidth", 0.5, "LineStyle", ":", "Color", "r")
grid on
xlim(t([1,end]))
xlabel("Time")
ylabel("Amplitude")
title("Impulse Response")

subplot 312
semilogx(f, H1Mag);
hold on
% semilogx(f, H2Mag);
hold off
xlim([2e1, 2e4])
ylim([-18, 12])
yticks(-18:6:18)
grid on
xlabel("Frequency")
ylabel("Amplitude")
title("Magnitude Response")
yyaxis right
semilogx(f, c, ":r", "LineWidth", 1)
ylim([0, 1])
yticks(0:0.2:1)

subplot 313
semilogx(f, H1Ang);
xlim([2e1, 2e4])
ylim([-180, 180])
yticks(-180:60:180)
grid on
xlabel("Frequency")
ylabel("Phase")
title("Phase Response")

h = h(startIdx:endIdx);
h = h .* tukeywin(length(h), 0.99);

audiowrite("CabinetIR.wav", h, fs);
