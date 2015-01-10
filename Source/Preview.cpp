/*
  ==============================================================================

    Preview.cpp
    Created: 28 Dec 2014 11:00:00am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Preview.h"

//==============================================================================
Preview::Preview()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Preview::~Preview()
{
    clearSlices();
}

void Preview::paint (Graphics& g)
{

    g.fillAll (Colours::black);   // clear the background

    g.setColour (Colours::lightblue);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("Preview", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Preview::addSlice( Slice* slice )
{
    SliceButton* newButton = new SliceButton ( slice );
    sliceButtons.add( newButton );

    if ( newButton->enabled )
        addAndMakeVisible( newButton );
    else
        addChildComponent( newButton );
}

void Preview::clearSlices()
{
    sliceButtons.clear();
}

void Preview::update(Slice* slice, int i)
{
    sliceButtons[i]->setVisible( slice->enabled );
}

void Preview::resized()
{
    
    for ( int i = 0; i < sliceButtons.size(); i++ )
    {
        sliceButtons[i]->setBoundsRelative(sliceButtons[i]->proportionalX, sliceButtons[i]->proportionalY, sliceButtons[i]->proportionalW, sliceButtons[i]->proportionalH);
    }

}

