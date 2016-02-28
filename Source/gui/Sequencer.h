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
class MainContentComponent;

//==============================================================================
/*
*/
class Sequencer    :	public Component,
						public Button::Listener,
						public Label::Listener,
						public Timer
{
public:
	Sequencer( MainContentComponent& parent );
    ~Sequencer();

    void paint (Graphics&) override;
    void resized() override;
    
    virtual void buttonClicked (Button*) override;
    virtual void buttonStateChanged (Button*) override;
	
	void labelTextChanged (Label* labelThatHasChanged) override;
    
    virtual void timerCallback() override;
    

	
	void nextStep();
	void previousStep();
	void selectStep( int i );
	
private:
	void addButton();
	void removeButton();
	
	void setButtonCount();
	
	MainContentComponent& parent;
    OwnedArray<Button> stepper;
    ScopedPointer<DrawableButton> play;
    ScopedPointer<DrawableButton> next;
    ScopedPointer<DrawableButton> previous;

	ScopedPointer<Button> lessSteps;
	ScopedPointer<Button> moreSteps;
	
    ScopedPointer<Label> sequenceName;
	ScopedPointer<Label> sequenceNumber;
	ScopedPointer<Label> stepNumber;
	
	void updateSequenceSettings ();
	
	ColourLookAndFeel claf;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};


#endif  // SEQUENCER_H_INCLUDED
