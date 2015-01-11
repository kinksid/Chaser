/*
  ==============================================================================

    Sequencer.h
    Created: 10 Jan 2015 7:14:27pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef SEQUENCER_H_INCLUDED
#define SEQUENCER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Sequencer    : public Component, public Button::Listener, public Timer, public ChangeBroadcaster
{
public:
    Sequencer();
    ~Sequencer();

    void paint (Graphics&);
    void resized();
    
    virtual void buttonClicked (Button*);
    virtual void buttonStateChanged (Button*);
    
    virtual void timerCallback();
    
    int activeButton;

private:
    OwnedArray<Button> stepper;
    ScopedPointer<Button> play;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};


#endif  // SEQUENCER_H_INCLUDED
