/* Chuck Example of a simple recorder */
PinkNoise src => Gain gOut => dac;
/* Send info to reference file */
gOut => WvOut refFile => blackhole;
/* Send info to measurement file */
adc => WvOut meaFile => blackhole;
// Define name of file
me.dir() + "MeasurementGuitarCabinet.wav" => string meaFilename;
me.dir() + "ReferenceGuitarCabinet.wav" => string refFilename;
// this is the output file name
refFilename => refFile.wavFilename;
meaFilename => meaFile.wavFilename;
/* Advance time */
10::second => now;
/* Close files */
refFile.closeFile();
meaFile.closeFile();
