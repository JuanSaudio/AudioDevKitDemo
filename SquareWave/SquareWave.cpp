#include <stdio.h>
#include <iostream>
#include <portaudio.h>
#include <math.h>
#include <assert.h>
#include <chrono>
#include <thread>

#define BUF_SIZE 512
#define SAMPLE_RATE 48000

// Define a structure for the synthesizer
struct SynthData {
    int nOsc;
    float fc;
    long lastSample;
    long totalSamples;
};

// Define an audio callback
int audioCallback(const void* inBuffer, void* outBuffer,
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void* userData)
{
    SynthData* synthData = (SynthData*)userData;
    float** out = (float**)outBuffer;
    int bufferSize = (int)framesPerBuffer;
    long ni = synthData->lastSample;
    float fc = synthData->fc;
    for (int n = 0; n < bufferSize; n++) {
        out[0][n] = out[1][n] = 0.0;
        for (int i = 0; i < synthData->nOsc; i++) {
            float t = double(ni) / double(SAMPLE_RATE);
            float curSamp = sin(2.0 * M_PI * fc * (i + 1) * t) / double(i + 1);
            out[0][n] += curSamp;
            out[1][n] += curSamp;
        }
        ni++;
    }
    synthData->lastSample = ni;
    if (synthData->lastSample > synthData->totalSamples)
        return paComplete;
    return paContinue;
}

// Main Function
int main(int argc, char* argv[]) {
    float dur = 1.0;
    SynthData synthData;
    // Define number of oscillators
    synthData.nOsc = 10;
    // Define Base Frequency
    synthData.fc = 220;
    synthData.lastSample = 0;
    // Pre compute the total number of samples
    synthData.totalSamples = SAMPLE_RATE * dur;

    PaStream* stream;
    PaError status;
    PaSampleFormat format = paFloat32 | paNonInterleaved;

    // Initialize Port audio
    status = Pa_Initialize();
    assert(status == paNoError);
    status = Pa_OpenDefaultStream(&stream, 0, 2, format,
                                  SAMPLE_RATE, BUF_SIZE,
                                  audioCallback, &synthData);
    assert(status == paNoError);
    status = Pa_StartStream(stream);
    assert(status == paNoError);
    std::string s;
    // Infinite loop
    while (Pa_IsStreamActive(stream)) {
         std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
