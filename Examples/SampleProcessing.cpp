// SampleProcessingExample in C++

class Biquad {
public:
    float process(float input){
        float output = z1 + input * b0;
        z1 = z2 + input * b1 - output * a1;
        z2 = input * b2 - output * a2;

        return output;
    }

private:
    float b0 = 1.0; float b1 = 0.0; float b2 = 0.0;
                    float a1 = 0.0; float a2 = 0.0;
    float z1 = 0.0; float z2 = 0.0;
};

Biquad biquad;

int numSamples = 1024;
float signal[numSamples];

for (int i = 0; i < numSamples; i++) {
    signal[i] = biquad.process(signal[i])
}
