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
	//sequenceLaf = new LookAndFeel_V1();
	
    
    for ( int i = 0; i < 16; i++ )
    {
        //create 16 buttons for the step sequencer
        Button* b = new TextButton( String (i + 1) );
		//b->setLookAndFeel(sequenceLaf);
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
    
    //sequence name label
    sequenceName = new Label("sequencename");
    sequenceName->setText(sequenceNames[activeSequence], dontSendNotification);
    sequenceName->setEditable(true);
	sequenceName->addListener(this);
    
    sequenceName->setColour(Label::outlineColourId, Colours::lightgrey);
    sequenceName->setColour(Label::backgroundColourId, Colours::darkgrey);
    sequenceName->setColour(Label::textColourId, Colours::whitesmoke);
    
    addAndMakeVisible(sequenceName);
	
	setDefaultSequenceNames();

    

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

StringArray Sequencer::getSequenceNames()
{
	return sequenceNames;
}

void Sequencer::setDefaultSequenceNames()
{
	sequenceNames.clear();
	for ( int i = 0; i < 16; i++ )
	{
		//fill the sequence names array with default names
		String seqName = "Sequence " + String ( i+1 );
		sequenceNames.add(seqName);
	}
	
	//update the label
	sequenceName->setText(sequenceNames[activeSequence], dontSendNotification);
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
	
	else if ( b == previous )
	{
		activeSequence--;
		if ( activeSequence < 0 )
			activeSequence = 15;
		sequenceName->setText(sequenceNames[activeSequence], dontSendNotification);
		stepper[0]->triggerClick();
	}
	
	else if ( b == next )
	{
		activeSequence++;
		if ( activeSequence > 15 )
			activeSequence = 0;
		sequenceName->setText(sequenceNames[activeSequence], dontSendNotification);
		stepper[0]->triggerClick();
	}
    
    else
    {
        activeButton = stepper.indexOf( b );
        if( b->getToggleState() )
            sendChangeMessage();
    }
}

void Sequencer::labelTextChanged (Label* labelThatHasChanged)
{
	sequenceNames.set(activeSequence, labelThatHasChanged->getText());
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
    float w = (0.835 / 0.98) / 16.0; //so the 16 buttons align perfectly with the preview window
    for(int i = 0; i < stepper.size(); i++ )
    {
        
        stepper[i]->setBoundsRelative( i * w, 0.0, w , 1.0 );
    }
    
    previous->setBoundsRelative(1.0-w*2.7, 0.2, w * 0.9, 0.8 );
    play->setBoundsRelative(1.0-w*1.8, 0.2, w * 0.9, 0.8 );
    next->setBoundsRelative(1.0-w*0.9, 0.2, w * 0.9, 0.8 );
    

    sequenceName->setBoundsRelative(1.0-w*2.65, 0.05, w * 2.6, 0.24);
    

}
