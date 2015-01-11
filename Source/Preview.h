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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Preview)
};


#endif  // PREVIEW_H_INCLUDED
