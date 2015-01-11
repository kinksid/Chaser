/*
  ==============================================================================

    XmlSequence.h
    Created: 11 Jan 2015 12:04:53am
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef XMLSEQUENCE_H_INCLUDED
#define XMLSEQUENCE_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"

//TODO add node with positions for all slices

class XmlSequence
{
public:
    XmlSequence();
    ~XmlSequence();
    
    void setStep ( int step, Array<int> activeSlices );
    Array<int> getStep ( int step );
    
private:
    ScopedPointer<XmlElement> sequence;
    OwnedArray<XmlElement> steps;
};



#endif  // XMLSEQUENCE_H_INCLUDED
