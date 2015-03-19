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
        b->setColour(TextButton::buttonColourId, Colours::darkgrey);
        b->setColour(TextButton::buttonOnColourId, claf.getOutlineColour());
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
	lessSteps = new TextButton( "less" );
	lessSteps->addListener( this );
	addAndMakeVisible( lessSteps );
	moreSteps = new TextButton( "more" );
	moreSteps->addListener( this );
	addAndMakeVisible( moreSteps );
    
    //sequence name label
    sequenceName = new Label("sequencename");
    sequenceName->setText(sequenceNames[activeSequence], dontSendNotification);
    sequenceName->setEditable(true);
	sequenceName->addListener(this);
    
    sequenceName->setColour(Label::outlineColourId, claf.getOutlineColour());
    sequenceName->setColour(Label::backgroundColourId, Colours::darkgrey);
    sequenceName->setColour(Label::textColourId, Colours::whitesmoke);
    
    addAndMakeVisible(sequenceName);
	
	setDefaultSequences();
}

Sequencer::~Sequencer()
{
    
}

void Sequencer::setSequenceNames(juce::StringArray seqNames)
{
	sequenceNames = seqNames;
	//update the label
	sequenceName->setText(sequenceNames[activeSequence], dontSendNotification );
}

/*
StringArray Sequencer::getSequenceNames()
{
	return sequenceNames;
}
 */

void Sequencer::setDefaultSequences()
{
	sequenceNames.clear();
	for ( int i = 0; i < 16; i++ )
	{
		//fill the sequence names array with default names
		String seqName = "Sequence " + String ( i+1 );
		sequenceNames.add(seqName);

		//set each sequence to be 16 steps
		numberOfSteps.add( 16 );
	}
	
	//update the label
	sequenceName->setText(sequenceNames[activeSequence], dontSendNotification);
	
	activeSequence = 0;
	activeButton = 0;
}

void Sequencer::paint (Graphics& g)
{
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
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
		resized();
		
		//let the listeners know
		Component::BailOutChecker checker (this);
		if (! checker.shouldBailOut())
			listeners.callChecked ( checker, &Listener::sequenceSelected, activeSequence );
		
		//always set the step to be the first step
		stepper[0]->triggerClick();
	}
	

	else if( b == lessSteps && numberOfSteps[activeSequence] > 1 )
	{
		//decrease the number of steps in the active sequence
		numberOfSteps.set( activeSequence, numberOfSteps[ activeSequence ] - 1 );
		resized();
		//if we were on a button that is no longer in range, trigger the previous step
		if( activeButton > numberOfSteps[ activeSequence ] - 1 )
			stepper[ numberOfSteps[ activeSequence ] -1 ]->triggerClick();
		
	}

	else if( b == moreSteps && numberOfSteps[ activeSequence ] < 16 )
	{
		//increase the number of steps in the active sequence
		numberOfSteps.set( activeSequence, numberOfSteps[ activeSequence ] + 1 );
		resized();
	}
    
    else
    {
        activeButton = stepper.indexOf( b );
        if( b->getToggleState() )
		{
			Component::BailOutChecker checker (this);
			if (! checker.shouldBailOut())
				listeners.callChecked ( checker, &Listener::stepSelected, activeButton );
		}
    }
}

void Sequencer::labelTextChanged (Label* labelThatHasChanged)
{
	sequenceNames.set(activeSequence, labelThatHasChanged->getText());
	
	Component::BailOutChecker checker (this);
	if (! checker.shouldBailOut())
		listeners.callChecked ( checker, &Listener::sequenceNameChanged, labelThatHasChanged->getText() );
	
}

void Sequencer::buttonStateChanged (Button* b)
{

}

void Sequencer::timerCallback()
{
    activeButton++;
    activeButton = fmod( activeButton, numberOfSteps[activeSequence] );
    stepper[activeButton]->triggerClick();
}

void Sequencer::resized()
{
	
	float w = (1.0 / float( numberOfSteps[ activeSequence ]) * 0.855); //the magic number math is there so the 16 buttons align perfectly with the preview window
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

	float sequenceControlButtonWidth = 0.0532526;
	previous->setBoundsRelative( 1.0 - sequenceControlButtonWidth * 2.7, 0.2, sequenceControlButtonWidth * 0.9, 0.8 );
	play->setBoundsRelative( 1.0 - sequenceControlButtonWidth * 1.8, 0.2, sequenceControlButtonWidth * 0.9, 0.8 );
	next->setBoundsRelative( 1.0 - sequenceControlButtonWidth * 0.9, 0.2, sequenceControlButtonWidth * 0.9, 0.8 );
    
	
	sequenceName->setBoundsRelative( 1.0 - sequenceControlButtonWidth * 2.65, 0.05, sequenceControlButtonWidth * 2.6, 0.24 );

	lessSteps->setBoundsRelative( 1.0 - sequenceControlButtonWidth * 2.7, 0.8, sequenceControlButtonWidth *  1.35, 0.15 );
	moreSteps->setBoundsRelative( 1.0 - sequenceControlButtonWidth * 1.4, 0.8, sequenceControlButtonWidth *  1.35, 0.15 );
}
