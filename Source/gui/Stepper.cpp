/*
  ==============================================================================

    Stepper.cpp
    Created: 28 Feb 2016 8:39:23pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Stepper.h"
#include "ColourLookAndFeel.h"
#include "../MainComponent.h"

//==============================================================================
Stepper::Stepper( ChaseManager& chaser ) : chaser( chaser )
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Stepper::~Stepper()
{
}

void Stepper::buttonClicked(juce::Button *b)
{
	if( b->getToggleState() )
	{
		chaser.skipToStep(stepper.indexOf( b ));
		findParentComponentOfClass<MainContentComponent>()->previewWindow->setActiveSlices( findParentComponentOfClass<MainContentComponent>()->chaseManager->getCurrentStep() );
	}
}

int Stepper::getButtonCount()
{
	return stepper.size();
}

void Stepper::triggerButton(int buttonIndex)
{
	if ( stepper.size() > buttonIndex && buttonIndex >= 0 )
		stepper[buttonIndex]->triggerClick();
}

void Stepper::addButton()
{
	//create a single for the step sequencer
	Button* b = new TextButton( String (stepper.size() + 1) );
	ColourLookAndFeel claf;
	b->setColour(TextButton::buttonColourId, claf.backgroundColour);
	b->setColour(TextButton::buttonOnColourId, claf.primaryColour);
	b->setRadioGroupId(1);
	b->setClickingTogglesState(true);
	b->addListener(this);
	addAndMakeVisible(b);
	stepper.add(b);
}

void Stepper::removeButton()
{
	stepper.removeLast();
}

void Stepper::setButtonCount()
{
	int count = chaser.getLastStepIndex() + 1;

	while ( stepper.size() < count )
		addButton();
	while ( stepper.size() > count )
		removeButton();
	
	//if we got out of the range
	//trigger the last available button
	//this will also update the chaseManager
	if ( chaser.getCurrentStepIndex() >= stepper.size() )
		stepper.getLast()->triggerClick();
	
	resized();
}

void Stepper::paint (Graphics& )
{
	
}

void Stepper::resized()
{
	float w = (1.0f / float( stepper.size() ));// * 0.817f); //the magic number math is there so the buttons align perfectly with the preview window
	for(int i = 0; i < stepper.size() ; i++ )
		stepper[ i ]->setBoundsRelative( i * w, 0.0, w, 1.0 );
}
