/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <faust/misc.h>
#include <faust/dsp/dsp.h>
#include <faust/gui/JuceParameterUI.h>
#include <faust/gui/JuceStateUI.h>
#include "FaustDistortion.h"

//==============================================================================
/**
*/

class DistortionAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DistortionAudioProcessor();
    ~DistortionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    faust::Distortion dist;
private:
    //==============================================================================
    juce::dsp::Convolution conv;
    JuceParameterUI paramUI;
    JuceStateUI stateUI;
    
    
    int uS = 1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessor)
};
