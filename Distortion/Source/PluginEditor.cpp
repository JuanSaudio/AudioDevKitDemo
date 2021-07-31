/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    audioProcessor.dist.buildUserInterface(&gui);
    addAndMakeVisible(gui);
    startTimerHz(30);
    setSize (400, 300);
    
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{
}

//==============================================================================
void DistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    using namespace juce;
    
    g.fillAll (Colour (0xFF2F3A40));

    {
        int x = 20, y = 19, width = 137, height = 35;
        String text (TRANS("Distortion"));
        Colour fillColour = Colours::black;
        g.setColour (fillColour);
        g.setFont (Font (30.60f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 19, y = 18, width = 137, height = 35;
        String text (TRANS("Distortion"));
        Colour fillColour = Colours::white;
        g.setColour (fillColour);
        g.setFont (Font (30.60f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        float x = 35.0f, y = 50.0f, width = 100.0f, height = 2.0f;
        Colour fillColour = Colours::aqua;
        g.setColour (fillColour);
        g.fillEllipse (x, y, width, height);
    }

    {
        int x = 35, y = 55, width = 61, height = 14;
        String text (TRANS("JuanSaudio"));
        Colour fillColour = Colour (0xfffbfbfb);
        g.setColour (fillColour);
        g.setFont (Font ("Futura (Light)", 12.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }
}

void DistortionAudioProcessorEditor::resized()
{
    auto box = getBounds();
    box.removeFromTop(80);
    gui.setBounds(box);
}


void DistortionAudioProcessorEditor::timerCallback(){
    gui.updateAllGuis();
}
