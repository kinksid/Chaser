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

//==============================================================================
/*
*/
class SliceButton    : public TextButton
{
public:
    SliceButton( String n, bool enable, double l, double t, double r, double b );
    ~SliceButton();
    
    double proportionalX;
    double proportionalY;
    double proportionalW;
    double proportionalH;
    bool enabled;

private:
    String name;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceButton)
};


#endif  // SLICEBUTTON_H_INCLUDED
