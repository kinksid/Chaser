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
    enabled = enable;
    proportionalX = l;
    proportionalY = t;
    proportionalW = r - l;
    proportionalH = b - t;

}

SliceButton::~SliceButton()
{
}


