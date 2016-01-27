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

    for ( int i = 0; i < 16; i++ )
    {
        //create 16 buttons for the step sequencer
        Button* b = new TextButton( String (i + 1) );
        b->setColour(TextButton::buttonColourId, claf.backgroundColour);
        b->setColour(TextButton::buttonOnColourId, claf.primaryColour);
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

	for( int i = 0; i < 16; i++ )
	{
		
	}
	
	
	//set the active vars and turn on the first step
    activeButton = 0;
	activeSequence = 0;
    stepper[0]->setToggleState(true, sendNotification);

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
    
    //sequence name label
    sequenceName = new Label("sequencename");
    sequenceName->setText(sequenceNames[activeSequence], dontSendNotification);
    sequenceName->setEditable(true);
	sequenceName->addListener(this);
    
    sequenceName->setColour(Label::outlineColourId, claf.outlineColour);
    sequenceName->setColour(Label::backgroundColourId, claf.backgroundColour);
    sequenceName->setColour(Label::textColourId, claf.textColour);
	sequenceName->setColour(TextEditor::ColourIds::highlightColourId, claf.primaryColour);
    
    addAndMakeVisible(sequenceName);
	
	//sequence number label
	sequenceNumber = new Label("sequencenumber");
	sequenceNumber->setText( "1:", dontSendNotification);
	sequenceNumber->setEditable(false);

	
	sequenceNumber->setColour(Label::outlineColourId, claf.outlineColour.darker());
	sequenceNumber->setColour(Label::backgroundColourId, claf.backgroundColour);
	sequenceNumber->setColour(Label::textColourId, claf.textColour.darker());

	
	addAndMakeVisible(sequenceNumber);
	
	setDefaultSequences();
}

Sequencer::~Sequencer()
{
    
}

void Sequencer::setSequenceLengths(Array<int> lengths)
{
	for ( int i = 0; i < lengths.size(); i++ )
	{
		if ( i < numberOfSteps.size() )
			numberOfSteps.set( i, lengths[i] );
	}
	
	//so the buttons get drawn again
	resized();
	
	//let the listeners know
	Component::BailOutChecker checker (this);
	if (! checker.shouldBailOut())
		listeners.callChecked ( checker, &Sequencer::Listener::sequenceLengthChanged, numberOfSteps[ activeSequence ] );
}

void Sequencer::setSequenceNames(juce::StringArray seqNames)
{
	sequenceNames = seqNames;
	//update the label
	sequenceName->setText(sequenceNames[activeSequence], dontSendNotification );
}

void Sequencer::setDefaultSequences()
{
	stopTimer();
	
	sequenceNames.clear();
	numberOfSteps.clear();
	
	for ( int i = 0; i < 16; i++ )
	{
		//fill the sequence names array with default names
		String seqName = "Sequence " + String ( i+1 );
		sequenceNames.add(seqName);

		//set each sequence to be 16 steps
		numberOfSteps.add( 16 );
	}
	
	activeSequence = 0;
	activeButton = 0;
	
	//update the label
	sequenceName->setText(sequenceNames[activeSequence], dontSendNotification);
	
	//let the listeners know
	Component::BailOutChecker checker (this);
	if (! checker.shouldBailOut())
	{
		listeners.callChecked ( checker, &Sequencer::Listener::sequenceSelected, activeSequence );
		listeners.callChecked ( checker, &Sequencer::Listener::sequenceLengthChanged, numberOfSteps[ activeSequence ] );
	}
	
	//always set the step to be the first step
	stepper[0]->triggerClick();
	
	resized();
}

void Sequencer::paint (Graphics& g)
{
    g.setColour (claf.backgroundColour);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void Sequencer::buttonClicked (Button* b)
{
    if ( b == play )
    {
        if ( b->getToggleState())
		{
			numberOfLoopsMade = 0;
            startTimer(200);
		}
        else
            stopTimer();
    }
	
	else if ( b == previous || b == next  )
	{
		//stop playing if it's playing
		if ( play->getToggleState() )
			play->triggerClick();
		
		if ( b == previous )
		{
			//only change the sequence when the first step is active
			if( activeButton == 0 )
			{
				activeSequence--;
				if( activeSequence < 0 )
					activeSequence = 15;
			}
		}
		
		else if ( b == next )
		{
			activeSequence++;
			if ( activeSequence > 15 )
				activeSequence = 0;
		}
		
		//update the sequence name and redraw the component
		sequenceName->setText( sequenceNames[ activeSequence ], dontSendNotification );
		sequenceNumber->setText( String(activeSequence + 1) + ": ", dontSendNotification);
		resized();
		
		//let the listeners know
		Component::BailOutChecker checker (this);
		if (! checker.shouldBailOut())
		{
			listeners.callChecked ( checker, &Sequencer::Listener::sequenceSelected, activeSequence );
			//the sequence lenght might also have changed
			listeners.callChecked ( checker, &Sequencer::Listener::sequenceLengthChanged, numberOfSteps[ activeSequence ] );
		}
		
		//always set the step to be the first step
		stepper[0]->triggerClick();
	}
	

	else if ( b == lessSteps || b == moreSteps )
	{
		if( b == lessSteps && numberOfSteps[activeSequence] > 1 )
		{
			//decrease the number of steps in the active sequence
			numberOfSteps.set( activeSequence, numberOfSteps[ activeSequence ] - 1 );
			
			//if we were on a button that is no longer in range, trigger the previous step
			if( activeButton > numberOfSteps[ activeSequence ] - 1 )
				stepper[ numberOfSteps[ activeSequence ] -1 ]->triggerClick();
		}

		else if( b == moreSteps && numberOfSteps[ activeSequence ] < 16 )
		{
			//increase the number of steps in the active sequence
			numberOfSteps.set( activeSequence, numberOfSteps[ activeSequence ] + 1 );
		}
		
		resized();
		//let the listeners know
		Component::BailOutChecker checker (this);
		if (! checker.shouldBailOut())
			listeners.callChecked ( checker, &Sequencer::Listener::sequenceLengthChanged, numberOfSteps[ activeSequence ] );
	}
    
    else
    {
        activeButton = stepper.indexOf( b );
        if( b->getToggleState() )
		{
			Component::BailOutChecker checker (this);
			if (! checker.shouldBailOut())
				listeners.callChecked ( checker, &Sequencer::Listener::stepSelected, activeButton );
		}
    }
}

void Sequencer::labelTextChanged (Label* labelThatHasChanged)
{
	sequenceNames.set(activeSequence, labelThatHasChanged->getText());
	
	Component::BailOutChecker checker (this);
	if (! checker.shouldBailOut())
		listeners.callChecked ( checker, &Sequencer::Listener::sequenceNameChanged, labelThatHasChanged->getText() );
	
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
	activeButton++;
	
	//rollover
	if ( activeButton > numberOfSteps[activeSequence] - 1)
	{
		activeButton = 0;
		numberOfLoopsMade++;
		
		//DJAktion is a little bitch
		if ( numberOfLoopsMade > 3 )
		{
			play->triggerClick();
		}
	}
	
	stepper[activeButton]->triggerClick();
}

void Sequencer::previousStep()
{
	activeButton--;
	if ( activeButton < 0 )
		activeButton = numberOfSteps[activeSequence] - 1;
	stepper[activeButton]->triggerClick();

}

void Sequencer::selectStep(int i)
{
	if ( i < numberOfSteps[activeSequence] && i > -1 )
	{
		activeButton = i;
		stepper[activeButton]->triggerClick();
	}
}

void Sequencer::resized()
{
	
	float w = (1.0f / float( numberOfSteps[ activeSequence ]) * 0.817f); //the magic number math is there so the 16 buttons align perfectly with the preview window
    for(int i = 0; i < stepper.size() ; i++ )
    {
		if( i < numberOfSteps[ activeSequence ] )
		{
			stepper[ i ]->setVisible( true );
			stepper[ i ]->setBoundsRelative( i * w, 0.0, w, 1.0 );
		}
		else
			stepper[ i ]->setVisible( false );
    }

	float sequenceControlButtonWidth = 0.0632526f;
	previous->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 2.7f, 0.225f, sequenceControlButtonWidth * 0.9f, 0.6f );
	play->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 1.8f, 0.225f, sequenceControlButtonWidth * 0.9f, 0.6f );
	next->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 0.9f, 0.225f, sequenceControlButtonWidth * 0.9f, 0.6f );
    
	sequenceNumber->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 2.65f, 0.05f, sequenceControlButtonWidth * 0.4f, 0.24f);
	sequenceName->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 2.2f, 0.05f, sequenceControlButtonWidth * 2.15f, 0.24f );

	lessSteps->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 2.7f, 0.75f, sequenceControlButtonWidth *  1.35f, 0.2f );
	moreSteps->setBoundsRelative( 1.0f - sequenceControlButtonWidth * 1.4f, 0.75f, sequenceControlButtonWidth *  1.35f, 0.2f );
}
