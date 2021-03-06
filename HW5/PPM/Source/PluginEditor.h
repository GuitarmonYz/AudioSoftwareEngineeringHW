/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MeterComponent.h"
//==============================================================================
/**
*/
class PpmAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    PpmAudioProcessorEditor (PpmAudioProcessor&);
    ~PpmAudioProcessorEditor();
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PpmAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PpmAudioProcessorEditor)
    std::vector<PpmBar*> rectangles;
    bool m_channelInitialized = false;
};
