% BufferProcessing example in matlab

numSamples = 2^16;
x = rand(numSamples, 1);

blockSize = 1024;
position = 0

while(position < numSamples)
    xi = x(position+1:position+blockSize)
    X = fft(xi)
    Y = conj(X)
    yi = ifft(Y)
    y(position+1:position+blockSize) = yi;
    position = position + blockSize;
end
