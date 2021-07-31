// Example in C


void reduceGainBy6dB(float* inData, float* outData, int nSamples) {
    for (int i = 0; i < nSamples; i++) {
        outData[i] = inData[i] / 2.0;
    }
}
