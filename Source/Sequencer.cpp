/*
  ==============================================================================

    Sequencer.cpp
    Created: 10 Jan 2015 7:14:27pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"

//==============================================================================
Sequencer::Sequencer()
{
    //create 16 buttons for the step sequencer
    for ( int i = 0; i < 16; i++ )
    {
        Button* b = new TextButton( String (i + 1) );
        b->setRadioGroupId(1);
        b->setClickingTogglesState(true);
        if ( i == 0 )
            b->setConnectedEdges(2);
        if ( i > 0 && i < 15 )
            b->setConnectedEdges(3);
        if ( i == 15 )
            b->setConnectedEdges(1);
        b->addListener(this);
        addAndMakeVisible(b);
        stepper.add(b);
    }
    
    activeButton = 0;
    stepper[0]->setToggleState(true, sendNotification);
    startTimer( 200 );

}

Sequencer::~Sequencer()
{
    
}

void Sequencer::paint (Graphics& g)
{
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void Sequencer::buttonClicked (Button* b)
{
    activeButton = stepper.indexOf( b );
    if( b->getToggleState() )
        sendChangeMessage();
}

void Sequencer::buttonStateChanged (Button* b)
{

}

void Sequencer::timerCallback()
{
    activeButton++;
    activeButton = fmod(activeButton, 16);
    stepper[activeButton]->triggerClick();
}

void Sequencer::resized()
{
    for(int i = 0; i < stepper.size(); i++ )
    {
        float w = 1.0 / 16.0;
        stepper[i]->setBoundsRelative( i * w, 0.0, w , 1.0 );
    }
    

}
