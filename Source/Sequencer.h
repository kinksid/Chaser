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
#include "ColourLookAndFeel.h"

//==============================================================================
/*
*/
class Sequencer    : public Component, public Button::Listener, public Label::Listener, public Timer//, public ChangeBroadcaster
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
    

	
	void setSequenceNames( StringArray seqNames );
	void setDefaultSequences();

	class Listener
	{
		
	public:
		virtual ~Listener() {}
		virtual void stepSelected ( int step ) = 0;
		virtual void sequenceNameChanged ( String newName ) = 0;
		virtual void sequenceSelected ( int sequence ) = 0;
		
	};
	
	void addListener ( Listener* const l ) { listeners.add ( l ); }
	void removeListener ( Listener* const l ) { listeners.remove (l ); }
	

private:
	int activeButton;
	int activeSequence;
	
    OwnedArray<Button> stepper;
    ScopedPointer<DrawableButton> play;
    ScopedPointer<DrawableButton> next;
    ScopedPointer<DrawableButton> previous;

	ScopedPointer<Button> lessSteps;
	ScopedPointer<Button> moreSteps;

	ListenerList<Listener> listeners;
    
	Array<int> numberOfSteps;
    ScopedPointer<Label> sequenceName;
    StringArray sequenceNames;
	
	ColourLookAndFeel claf;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};


#endif  // SEQUENCER_H_INCLUDED
