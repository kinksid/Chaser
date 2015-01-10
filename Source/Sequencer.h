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
class Sequencer    : public Component
{
public:
    Sequencer();
    ~Sequencer();

    void paint (Graphics&);
    void resized();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};


#endif  // SEQUENCER_H_INCLUDED
