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
}

void Preview::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::black);   // clear the background

    g.setColour (Colours::lightblue);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("Preview", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Preview::addSlice(SliceButton *slice)
{
    SliceButton* newButton = slice;
    slices.add(newButton);
    if ( newButton->enabled )
        addAndMakeVisible( newButton );
    else
        addChildComponent( newButton );
}

void Preview::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    for ( int i = 0; i < slices.size(); i++ )
    {
        slices[i]->setBoundsRelative(slices[i]->proportionalX, slices[i]->proportionalY, slices[i]->proportionalW, slices[i]->proportionalH);
    }

}
