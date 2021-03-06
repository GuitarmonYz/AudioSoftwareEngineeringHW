/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class VibratopluginAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    VibratopluginAudioProcessorEditor (VibratopluginAudioProcessor&);
    ~VibratopluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
//    void sliderValueChanged (Slider* sliderThatWasMoved) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VibratopluginAudioProcessor& processor;
    ScopedPointer<Button> bypassButton;
    ScopedPointer<Slider> widthSlider, freqSlider;
    ScopedPointer<Label> widthLabel, freqLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratopluginAudioProcessorEditor)
};
