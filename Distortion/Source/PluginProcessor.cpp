/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

std::list<GUI*> GUI::fGuiList;

//==============================================================================
DistortionAudioProcessor::DistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
, paramUI(this)
{
    dist.buildUserInterface(&paramUI);
    dist.buildUserInterface(&stateUI);
}

DistortionAudioProcessor::~DistortionAudioProcessor()
{
}

//==============================================================================
const juce::String DistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionAudioProcessor::getNumPrograms()
{
    return 1;
}

int DistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    using namespace juce;
    using namespace dsp;
    using namespace BinaryData;
    ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    conv.prepare(spec);
    dist.init(sampleRate);
    conv.loadImpulseResponse(CabinetIR_wav,
                             CabinetIR_wavSize,
                             Convolution::Stereo::no,
                             Convolution::Trim::no, 0);
}

void DistortionAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannels() != 1)
        return false;
    if (layouts.getMainOutputChannels() != 1)
        return false;
    return true;
}
#endif

void DistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    using namespace juce;
    using namespace dsp;
    float** data = buffer.getArrayOfWritePointers();
    int nSamps = buffer.getNumSamples();
    dist.compute(nSamps, data, data);
    ScopedNoDenormals noDenormals;
    AudioBlock<float> block(buffer);
    ProcessContextReplacing<float> ctxt(block);
    conv.process(ctxt);
}

//==============================================================================
bool DistortionAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DistortionAudioProcessor::createEditor()
{
    return new DistortionAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    stateUI.getStateInformation(destData);
}

void DistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    stateUI.setStateInformation(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAudioProcessor();
}
