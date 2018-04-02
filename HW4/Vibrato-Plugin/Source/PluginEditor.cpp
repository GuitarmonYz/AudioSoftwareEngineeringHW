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
    addAndMakeVisible(widthSlider = new Slider("Width"));
    widthSlider->setRange(0, 1000, 1);
    widthSlider->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    widthSlider->setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    widthSlider->setTextValueSuffix("ms");
    widthSlider->onValueChange = [this] {
       processor.m_Vibrato->setParam(CVibrato::kParamModWidthInS, widthSlider->getValue()/1000);
    };
    
    addAndMakeVisible(widthLabel = new Label("Width"));
    widthLabel->setText("Width", dontSendNotification);
    widthLabel->attachToComponent(widthSlider, false);
    
    addAndMakeVisible(freqSlider = new Slider("Freq"));
    freqSlider->setRange(0.0f, 20.0f, 0.1);
    freqSlider->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    freqSlider->setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    freqSlider->setTextValueSuffix("Hz");
    freqSlider->onValueChange = [this] {
        processor.m_Vibrato->setParam(CVibrato::kParamModFreqInHz, freqSlider->getValue());
    };
    
    addAndMakeVisible(freqLabel = new Label("Freq"));
    freqLabel->setText("Freq", dontSendNotification);
    freqLabel->attachToComponent(freqSlider, false);
    
    addAndMakeVisible(bypassButton = new ToggleButton("Bypass"));
    bypassButton->onClick = [this] {
        processor.toggleBypass(freqSlider->getValue());
    };
    
    widthSlider->setValue(processor.m_Vibrato->getParam(CVibrato::kParamModWidthInS)*1000);
    freqSlider->setValue(processor.m_Vibrato->getParam(CVibrato::kParamModFreqInHz));
    
    setSize (400, 247.2);
    
}

VibratopluginAudioProcessorEditor::~VibratopluginAudioProcessorEditor()
{
    widthSlider = nullptr;
    widthLabel = nullptr;
    freqSlider = nullptr;
    freqLabel = nullptr;
    
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
    freqSlider->setBounds (proportionOfWidth (0), proportionOfHeight (0.3f), proportionOfWidth (0.4f), proportionOfHeight (0.4f));
    widthSlider->setBounds (proportionOfWidth (0.5f), proportionOfHeight (0.3f), proportionOfWidth (0.4f), proportionOfHeight (0.4f));
    bypassButton->setBoundsRelative(0.8f, 0, 0.2f, 0.2f);
}
