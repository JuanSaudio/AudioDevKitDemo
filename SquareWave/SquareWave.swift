import Foundation
import AVFoundation

struct SynthData {
    let nOsc: Int
    let fc: Float
    var totalSamples: Int
    var lastSample: Int = 0
}

let engine = AVAudioEngine()
let mainMixer = engine.mainMixerNode
let output = engine.outputNode
let outputFormat = output.inputFormat(forBus: 0)
let sampleRate = Float(outputFormat.sampleRate)
let inputFormat = AVAudioFormat(commonFormat: outputFormat.commonFormat,
                                sampleRate: outputFormat.sampleRate,
                                channels: 1,
                                interleaved: outputFormat.isInterleaved)
let dur: Float = 1.0
var synthData = SynthData(nOsc: 10, fc: 100, totalSamples: Int(dur * sampleRate))

let srcNode = AVAudioSourceNode { _, _, frameCount, audioBufferList -> OSStatus in
    let ablPointer = UnsafeMutableAudioBufferListPointer(audioBufferList)
    for frame in 0..<Int(frameCount) {
        var value: Float = 0.0
        for i in 1...synthData.nOsc {
            value += sin(2 * Float.pi * synthData.fc * Float(i) * Float(synthData.lastSample) / sampleRate) / Float(i)
        }
        synthData.lastSample += 1
        for buffer in ablPointer {
            let buf: UnsafeMutableBufferPointer<Float> = UnsafeMutableBufferPointer(buffer)
            buf[frame] = value
        }
        if synthData.lastSample >= synthData.totalSamples {
            CFRunLoopStop(CFRunLoopGetMain())
        }
    }
    return noErr
}

engine.attach(srcNode)
engine.connect(srcNode, to: mainMixer, format: inputFormat)
engine.connect(mainMixer, to: output, format: outputFormat)
mainMixer.outputVolume = 0.5

do {
    try engine.start()
    CFRunLoopRun()
    engine.stop()
} catch {
    print("Could not start engine: \(error)")
}
