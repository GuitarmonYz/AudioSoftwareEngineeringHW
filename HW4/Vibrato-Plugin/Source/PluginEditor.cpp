/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VibratopluginAudioProcessorEditor::VibratopluginAudioProcessorEditor (VibratopluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(sliderWidth = new Slider("Width"));
    sliderWidth->setRange(0, 1000, 1);
    sliderWidth->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sliderWidth->setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    sliderWidth->setTextValueSuffix("ms");
    sliderWidth->onValueChange = [this] {
       processor.m_Vibrato->setParam(CVibrato::kParamModWidthInS, sliderWidth->getValue()/1000);
    };
//    sliderWidth->addListener(this);
    
    addAndMakeVisible(sliderFreq = new Slider("Freq"));
    sliderFreq->setRange(0.0f, 20.0f, 0.1);
    sliderFreq->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sliderFreq->setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    sliderFreq->setTextValueSuffix("Hz");
    sliderFreq->onValueChange = [this] {
        processor.m_Vibrato->setParam(CVibrato::kParamModFreqInHz, sliderFreq->getValue());
    };
//    sliderFreq->addListener(this);
    
    addAndMakeVisible(bypassButton = new ToggleButton("Bypass"));
    bypassButton->onStateChange = [this] {
        processor.toggleBypass();
    };
    
    
    sliderWidth->setValue(processor.m_Vibrato->getParam(CVibrato::kParamModWidthInS)*1000);
    sliderFreq->setValue(processor.m_Vibrato->getParam(CVibrato::kParamModFreqInHz));
    
    setSize (400, 300);
    
}

VibratopluginAudioProcessorEditor::~VibratopluginAudioProcessorEditor()
{
    sliderWidth = nullptr;
    sliderFreq = nullptr;
}

//==============================================================================
void VibratopluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void VibratopluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    sliderFreq->setBounds (proportionOfWidth (0.1f), proportionOfHeight (0), proportionOfWidth (0.4f), proportionOfHeight (0.4f));
    sliderWidth->setBounds (proportionOfWidth (0.1f), proportionOfHeight (0.6f), proportionOfWidth (0.4f), proportionOfHeight (0.4f));
    bypassButton->setBoundsRelative(0.1f, 0.8f, 0.2f, 0.2f);
}
