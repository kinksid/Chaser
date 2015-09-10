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


//==============================================================================
/*
*/

class SliceButton    : public ShapeButton
{
public:
    SliceButton ( String n, bool enable, double l, double t, double r, double b );
    SliceButton ( Slice* s, Point<int> scale );
    ~SliceButton();
    
    double proportionalX;
    double proportionalY;
    double proportionalW;
    double proportionalH;
    bool enabled;
    String name;
    
    void update();
	Path getPath()
	{
		if ( mask.isEmpty())
			return path;
		else
			return mask;
	}
	
	bool hitTest(int x, int y) override
	{
		DBG(x);
		DBG(y);
		if ( getPath().contains( x+getPosition().x, y+getPosition().y))
			return true;
		return false;
	}
	
private:
	Path path;
	Path mask;
	Path maskRect;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceButton)
};


#endif  // SLICEBUTTON_H_INCLUDED
