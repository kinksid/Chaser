/*
  ==============================================================================

    Copier.cpp
    Created: 1 Feb 2015 3:36:41pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Copier.h"

//==============================================================================
Copier::Copier()
{
	//create 4 buttons for x1, x2, x4 and x8
	for ( int i = 0; i < 4; i++ )
	{
		TextButton* b = new TextButton(String(i));
		b->setButtonText("x" + String(pow(2,i)));
		b->setColour(TextButton::buttonColourId, Colours::darkgrey);
		b->addListener(this);
		addAndMakeVisible(b);
		buttons.add(b);
	}
}

Copier::~Copier()
{
}

void Copier::buttonClicked(Button* b)
{
	clickedButton = b;
	sendChangeMessage();
}

void Copier::paint (Graphics& g)
{
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void Copier::resized()
{
	float h = 1.0 / buttons.size() - 0.01;
	for ( int i = 0; i < buttons.size(); i++ )
	{
		buttons[i]->setBoundsRelative(0.01, 0.01 + i * h + 0.01, 0.98, h);
	}
}
