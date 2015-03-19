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
#include "ColourLookAndFeel.h"


//==============================================================================
/*
*/

class SliceLookAndFeel : public ColourLookAndFeel
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
        
		g.setColour (getOutlineColour().withAlpha ((isMouseOverButton) ? 1.0f : 0.8f));
        g.strokePath (p, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.4f));
    }
    
};


class Preview    : public Component, public Button::Listener
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
	
	class Listener
	{
		
	public:
		virtual ~Listener() {}
		virtual void sliceClicked ( Array<int> ) = 0;
		
	};
	
	void addListener ( Listener* const l ) { previewListeners.add ( l ); }
	void removeListener ( Listener* const l ) { previewListeners.remove (l ); }

    

private:
    OwnedArray<SliceButton> sliceButtons;
    ScopedPointer<SliceLookAndFeel> sliceLaf;
	ListenerList<Listener> previewListeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Preview)
};


#endif  // PREVIEW_H_INCLUDED
