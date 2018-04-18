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
    setSize (700, 300);
    
    
    
    startTimer(10);
}

PpmAudioProcessorEditor::~PpmAudioProcessorEditor()
{
    for (int i = 0; i < processor.getNumOfChannels(); i++) {
        delete rectangles.at(i);
    }
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
    if (!m_channelInitialized) {
        for (int i = 0; i < processor.getNumOfChannels(); i++) {
            PpmBar* pPb = new PpmBar();
            rectangles.push_back(pPb);
            addAndMakeVisible(pPb);
        }
        m_channelInitialized = true;
    }
    
    for (int i = 0; i < processor.getNumOfChannels(); i++) {
        rectangles.at(i)->setBounds(40 * i + 20, 20, 35, 200);
    }
}

void PpmAudioProcessorEditor::timerCallback() {
    float* PpmVals = processor.getCurPpmVal();
    for (int i = 0; i < processor.getNumOfChannels(); i++) {
        rectangles.at(i)->setValue(PpmVals[i]);
    }
}
