/*
  ==============================================================================

    SliceButton.cpp
    Created: 28 Dec 2014 11:15:33am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceButton.h"

//==============================================================================
SliceButton::SliceButton(String n, bool enable, double l, double t, double r, double b)
{
    setButtonText( n );
    
    //enable loads the state from the ass xml file
    //it can still be edited via the slicelist
    enabled = enable;
    
    setClickingTogglesState( true );
    setToggleState( false, sendNotification );
    
    proportionalX = l;
    proportionalY = t;
    proportionalW = r - l;
    proportionalH = b - t;

}

SliceButton::SliceButton(Slice* s) : SliceButton(s->name, s->enabled, s->proportionalX, s->proportionalY, s->proportionalW + s->proportionalX, s->proportionalH + s->proportionalY)
{

}

SliceButton::~SliceButton()
{
    
}




