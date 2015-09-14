/*
  ==============================================================================

    SliceButton.h
    Created: 28 Dec 2014 11:15:33am
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef SLICEBUTTON_H_INCLUDED
#define SLICEBUTTON_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Slice.h"
#include "ColourLookAndFeel.h"


//==============================================================================
/*
*/

class SliceButton    : public ShapeButton
{
public:
	//SliceButton ( String n, bool enable, double l, double t, double r, double b );
    SliceButton ( Slice* s, Point<int> scale );
    ~SliceButton();
    
    bool enabled;
    String name;
    
    void update();
	Path getPath();
	
	//overridden functions for ShapeButton
	bool hitTest(int x, int y) override;
	void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
	Path path;
	Path mask;
	Path maskRect;
	
	Path makePath( Array<Point<float>>&, Point<int> scale );
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceButton)
};


#endif  // SLICEBUTTON_H_INCLUDED
