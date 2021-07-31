PinkNoise src => Gain gOut => dac;
gOut => WvOut refFile => blackhole;
adc => WvOut meaFile => blackhole;
// Define name of file
me.dir() + "MeasurementGuitarCabinet.wav" => string meaFilename;
me.dir() + "ReferenceGuitarCabinet.wav" => string refFilename;
// this is the output file name
refFilename => refFile.wavFilename;
meaFilename => meaFile.wavFilename;
10::second => now;
refFile.closeFile();
meaFile.closeFile();
