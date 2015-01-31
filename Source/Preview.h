/*
  ==============================================================================

    Preview.h
    Created: 28 Dec 2014 11:00:00am
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef PREVIEW_H_INCLUDED
#define PREVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceButton.h"

//==============================================================================
/*
*/

class SliceLookAndFeel : public LookAndFeel_V1
{
public:
    
    SliceLookAndFeel(){}
    ~SliceLookAndFeel(){}
    //override the button drawing function
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                                               bool isMouseOverButton, bool isButtonDown)
    {
        const int width = button.getWidth();
        const int height = button.getHeight();
        
        const float indent = 2.0f;
        
        Path p;
        p.addRectangle (indent, indent,
                        width - indent * 2.0f,
                        height - indent * 2.0f);
        
        Colour bc (backgroundColour.withMultipliedSaturation (0.3f));
        
        if (isMouseOverButton)
        {
            if (isButtonDown)
                bc = bc.brighter();
            else if (bc.getBrightness() > 0.5f)
                bc = bc.darker (0.1f);
            else
                bc = bc.brighter (0.1f);
        }
        
        g.setColour (bc);
        g.fillPath (p);
        
        g.setColour (bc.contrasting().withAlpha ((isMouseOverButton) ? 0.6f : 0.4f));
        g.strokePath (p, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.4f));
    }
    
};


class Preview    : public Component, public Button::Listener, public ChangeBroadcaster
{
public:
    Preview();
    ~Preview();

    void paint (Graphics&);
    void resized();
    
    virtual void buttonClicked (Button*);
    
    void update(Slice* slice, int i );
    
    void clearSlices();
    void addSlice( Slice* slice);
    
    void setSlices ( Array<int> activeSlices );
    Array<int> getSlices ();
    

    

private:
    OwnedArray<SliceButton> sliceButtons;
    ScopedPointer<SliceLookAndFeel> sliceLaf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Preview)
};


#endif  // PREVIEW_H_INCLUDED
