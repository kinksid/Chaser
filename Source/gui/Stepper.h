/*
  ==============================================================================

    Stepper.h
    Created: 28 Feb 2016 8:39:23pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef STEPPER_H_INCLUDED
#define STEPPER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../chase/ChaseManager.h"

//==============================================================================
/*
*/
class Stepper    :  public Component,
					public Button::Listener
{
public:
    Stepper( ChaseManager& chaser );
    ~Stepper();

    void paint (Graphics&) override;
    void resized() override;
	
	virtual void buttonClicked (Button*) override;
	
	void setButtonCount();
	int getButtonCount();
	
	void triggerButton ( int buttonIndex );

private:
	void addButton();
	void removeButton();
	
	
	OwnedArray<Button> stepper;
	ChaseManager& chaser;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Stepper)
};


#endif  // STEPPER_H_INCLUDED
