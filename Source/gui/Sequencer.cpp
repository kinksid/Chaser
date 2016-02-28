/*
  ==============================================================================

    Sequencer.cpp
    Created: 10 Jan 2015 7:14:27pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"
#include "MainComponent.h"


//==============================================================================
Sequencer::Sequencer( MainContentComponent& parent ): parent ( parent )
{
    //drawable svg buttons
    ScopedPointer<XmlElement> drawPlayXml (XmlDocument::parse (BinaryData::play_svg));
    ScopedPointer<XmlElement> drawPauseXml (XmlDocument::parse (BinaryData::pause_svg));
    ScopedPointer<XmlElement> drawNextXml (XmlDocument::parse (BinaryData::next_svg));
    ScopedPointer<XmlElement> drawPrevXml (XmlDocument::parse (BinaryData::previous_svg));

    ScopedPointer<Drawable>  drawPlay = Drawable::createFromSVG (*drawPlayXml);
    ScopedPointer<Drawable>  drawPause = Drawable::createFromSVG (*drawPauseXml);
    play = new DrawableButton("play", DrawableButton::ButtonStyle::ImageFitted );
    play->setColour(DrawableButton::backgroundOnColourId, Colours::transparentBlack);
    play->setImages(drawPlay, drawPlay, drawPause, drawPlay, drawPause);
    play->setEdgeIndent(12);
    play->setClickingTogglesState(true);
    addAndMakeVisible(play);
    play->addListener(this);

    ScopedPointer<Drawable> drawNext = Drawable::createFromSVG (*drawNextXml);
    next = new DrawableButton("next", DrawableButton::ButtonStyle::ImageFitted );
    next->setImages(drawNext);
    next->setEdgeIndent(10);
    addAndMakeVisible(next);
    next->addListener(this);

    ScopedPointer<Drawable> drawPrev = Drawable::createFromSVG (*drawPrevXml);
    previous = new DrawableButton("previous", DrawableButton::ButtonStyle::ImageFitted );
    previous->setImages(drawPrev);
    previous->setEdgeIndent(10);
    addAndMakeVisible(previous);
    previous->addListener(this);

	//buttons to add and remove steps
	lessSteps = new TextButton( "-" );
	lessSteps->setColour(TextButton::buttonColourId, claf.backgroundColour);
	lessSteps->addListener( this );
	addAndMakeVisible( lessSteps );
	moreSteps = new TextButton( "+" );
	moreSteps->addListener( this );
	moreSteps->setColour(TextButton::buttonColourId, claf.backgroundColour);
	addAndMakeVisible( moreSteps );
	
	//step number label
	stepNumber = new Label("stepNumber");
	stepNumber->addListener( this );
	stepNumber->setEditable( true );
	stepNumber->setColour(Label::outlineColourId, claf.outlineColour.darker());
	stepNumber->setColour(Label::backgroundColourId, claf.backgroundColour);
	stepNumber->setColour(Label::textColourId, claf.textColour.darker());
	stepNumber->setColour(TextEditor::ColourIds::highlightColourId, claf.primaryColour);
	addAndMakeVisible(stepNumber);
    
    //sequence name label
    sequenceName = new Label("sequencename");
    sequenceName->setEditable(true);
	sequenceName->addListener(this);
    sequenceName->setColour(Label::outlineColourId, claf.outlineColour);
    sequenceName->setColour(Label::backgroundColourId, claf.backgroundColour);
    sequenceName->setColour(Label::textColourId, claf.textColour);
	sequenceName->setColour(TextEditor::ColourIds::highlightColourId, claf.primaryColour);
    addAndMakeVisible(sequenceName);
	
	//sequence number label
	sequenceNumber = new Label("sequencenumber");
	sequenceNumber->addListener( this );
	sequenceNumber->setEditable( true );
	sequenceNumber->setColour(Label::outlineColourId, claf.outlineColour.darker());
	sequenceNumber->setColour(Label::backgroundColourId, claf.backgroundColour);
	sequenceNumber->setColour(Label::textColourId, claf.textColour.darker());
	sequenceNumber->setColour(TextEditor::ColourIds::highlightColourId, claf.primaryColour);
	addAndMakeVisible(sequenceNumber);

	updateSequenceSettings();
}


Sequencer::~Sequencer()
{
	
}


void Sequencer::paint (Graphics& g)
{

}

void Sequencer::buttonClicked (Button* b)
{
    if ( b == play )
    {
        if ( b->getToggleState())
            startTimer(200);
        else
            stopTimer();
    }
	
	else if ( b == previous || b == next  )
	{
		//stop playing if it's playing
		if ( play->getToggleState() )
			play->triggerClick();
		
		if ( b == previous )
			parent.chaseManager->skipToPreviousSequence();

		
		else if ( b == next )
			parent.chaseManager->skipToNextSequence();
		
		updateSequenceSettings();
	}
	

	else if ( b == lessSteps || b == moreSteps )
	{
		if( b == lessSteps )
			parent.chaseManager->removeStep();

		else if ( b == moreSteps )
			parent.chaseManager->addStep();
		
		updateSequenceSettings();
	}
    else
    {
        if( b->getToggleState() )
		{
			parent.chaseManager->skipToStep(stepper.indexOf( b ));
			parent.previewWindow->setActiveSlices( parent.chaseManager->getCurrentStepSlices() );
		}
    }
}

void Sequencer::labelTextChanged (Label* labelThatHasChanged)
{
	if ( labelThatHasChanged == sequenceName )
		parent.chaseManager->setCurrentSequenceName( labelThatHasChanged->getText() );
	
	else if ( labelThatHasChanged == sequenceNumber )
	{
		int sequenceToJumpTo = sequenceNumber->getText().getIntValue();
		if ( sequenceToJumpTo > 0  )
			parent.chaseManager->skipToSequence( sequenceToJumpTo );
		
		//calling update here will make sure the values in the labels will update correctly, even if an unaccepted value was entered
		updateSequenceSettings();
	}
	
	else if ( labelThatHasChanged == stepNumber )
	{
		int desiredStepCount = stepNumber->getText().getIntValue();
		if ( desiredStepCount > 0 )
			parent.chaseManager->setStepCount( desiredStepCount );
		updateSequenceSettings();
	}
}

void Sequencer::buttonStateChanged (Button* b)
{

}

void Sequencer::timerCallback()
{
	nextStep();
}

void Sequencer::nextStep()
{
	int nextStep = parent.chaseManager->skipToNextStep();
	stepper[nextStep]->triggerClick();
}

void Sequencer::previousStep()
{
	int previousStep = parent.chaseManager->skipToPreviousStep();
	stepper[previousStep]->triggerClick();
}

void Sequencer::selectStep(int i)
{
	parent.chaseManager->skipToStep( i );
}

void Sequencer::addButton()
{
	//create a single for the step sequencer
	Button* b = new TextButton( String (stepper.size() + 1) );
	b->setColour(TextButton::buttonColourId, claf.backgroundColour);
	b->setColour(TextButton::buttonOnColourId, claf.primaryColour);
	b->setRadioGroupId(1);
	b->setClickingTogglesState(true);
	b->addListener(this);
	addAndMakeVisible(b);
	stepper.add(b);
}

void Sequencer::removeButton()
{
	stepper.removeLast();
}

void Sequencer::setButtonCount()
{
	int count = parent.chaseManager->getLastStepIndex() + 1;
//	if ( count > 1 ) //can't have less than one button
//	{
		while ( stepper.size() < count )
			addButton();
		while ( stepper.size() > count )
			removeButton();
		
		//if we got out of the range
		//trigger the last available button
		//this will also update the chaseManager
		if ( parent.chaseManager->getCurrentStepIndex() >= stepper.size() )
			stepper.getLast()->triggerClick();
		
		resized();
//	}
}

void Sequencer::updateSequenceSettings()
{
	//check that we have enough steps
	setButtonCount();
	
	//update the stepnumber label
	stepNumber->setText( String(parent.chaseManager->getLastStepIndex() + 1), dontSendNotification );
	
	//retrigger the current step
	stepper[parent.chaseManager->getCurrentStepIndex()]->triggerClick();
	
	//update the sequence name component
	sequenceName->setText( parent.chaseManager->getCurrentSequenceName(), dontSendNotification);
	sequenceNumber->setText(String( parent.chaseManager->getCurrentSequenceIndex() + 1), dontSendNotification );
	
}

void Sequencer::resized()
{
	
	float w = (1.0f / float( stepper.size() ) * 0.817f); //the magic number math is there so the buttons align perfectly with the preview window
    for(int i = 0; i < stepper.size() ; i++ )
    {
		stepper[ i ]->setBoundsRelative( i * w, 0.0, w, 1.0 );
    }

	float sequenceControlButtonWidth = 0.0632526f;
	previous->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 2.7f, 0.225f, sequenceControlButtonWidth * 0.9f, 0.6f );
	play->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 1.8f, 0.225f, sequenceControlButtonWidth * 0.9f, 0.6f );
	next->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 0.9f, 0.225f, sequenceControlButtonWidth * 0.9f, 0.6f );
    
	sequenceNumber->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 2.65f, 0.05f, sequenceControlButtonWidth * 0.4f, 0.24f);
	sequenceName->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 2.2f, 0.05f, sequenceControlButtonWidth * 2.15f, 0.24f );

	lessSteps->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 2.7f, 0.75f, sequenceControlButtonWidth, 0.2f );
	moreSteps->setBoundsRelative( 1.0f - sequenceControlButtonWidth, 0.75f, sequenceControlButtonWidth, 0.2f );
	
	stepNumber->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 1.65f, 0.775f, sequenceControlButtonWidth * 0.6f, 0.15f );
}
