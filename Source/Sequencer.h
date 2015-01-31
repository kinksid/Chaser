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
class Sequencer    : public Component, public Button::Listener, public Label::Listener, public Timer, public ChangeBroadcaster
{
public:
    Sequencer();
    ~Sequencer();

    void paint (Graphics&);
    void resized();
    
    virtual void buttonClicked (Button*);
    virtual void buttonStateChanged (Button*);
	
	virtual void labelTextChanged (Label* labelThatHasChanged);
    
    virtual void timerCallback();
    
    int activeButton;
    int activeSequence;
	
	void setSequenceNames( StringArray seqNames )
	{
		sequenceNames = seqNames;
		//update the label
		sequenceName->setText(sequenceNames[activeSequence], dontSendNotification );
	}
	
	StringArray getSequenceNames()
	{
		return sequenceNames;
	}

private:
    OwnedArray<Button> stepper;
    ScopedPointer<DrawableButton> play;
    ScopedPointer<DrawableButton> next;
    ScopedPointer<DrawableButton> previous;
    
    ScopedPointer<LookAndFeel_V1> sequenceLaf;
    ScopedPointer<Label> sequenceName;
    StringArray sequenceNames;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};


#endif  // SEQUENCER_H_INCLUDED
