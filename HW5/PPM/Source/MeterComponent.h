/*
  ==============================================================================

    PpmBar.h
    Created: 17 Apr 2018 8:55:55pm
    Author:  zhao yan

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>
#include <iostream>

//==============================================================================
/*
*/
class PpmBar    : public Component
{
public:
    PpmBar()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        m_barHight = 0;
    }

    ~PpmBar()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */
        g.fillAll (Colours::black);   // clear the background
        if (m_barHight > -12) {
            g.setColour(Colours::green);
            int pos_x = 0;
            int pos_y = getHeight() * (-m_barHight/12);
            int width = getWidth();
            int height = getHeight() - pos_y;
            g.fillRect(pos_x, pos_y, width, height);
            g.drawRect(pos_x, pos_y, width, height);
        }
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
    }
    
    void setValue(float ppmValue) {
        std::cout<<ppmValue<<std::endl;
        m_barHight = 20 * log10(ppmValue);
//        std::cout<<m_barHight<<std::endl;
        repaint();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PpmBar)
    float m_barHight;
};
