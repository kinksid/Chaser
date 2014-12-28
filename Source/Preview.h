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
class Preview    : public Component
{
public:
    Preview();
    ~Preview();

    void paint (Graphics&);
    void resized();
    
    void clearSlices()
    {
        slices.clear();
    }
    void addSlice( SliceButton* slice );

    

private:
    OwnedArray<SliceButton> slices;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Preview)
};


#endif  // PREVIEW_H_INCLUDED
