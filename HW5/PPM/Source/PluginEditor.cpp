/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PpmAudioProcessorEditor::PpmAudioProcessorEditor (PpmAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(rectangle);
    startTimer(10);
}

PpmAudioProcessorEditor::~PpmAudioProcessorEditor()
{
    stopTimer();
    
}

//==============================================================================
void PpmAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void PpmAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    rectangle.setBounds(0, 0, 50, 200);
}

void PpmAudioProcessorEditor::timerCallback() {
//    rectangle.setValue(processor.getCurPpmVal());
    float* PpmVals = processor.getCurPpmVal();
    rectangle.setValue(PpmVals[0]);
}
