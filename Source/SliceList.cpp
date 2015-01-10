/*
  ==============================================================================

    SliceList.cpp
    Created: 10 Jan 2015 7:14:14pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceList.h"

//==============================================================================
SliceList::SliceList()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    sliceList.setModel( this );
    sliceList.setColour(ListBox::backgroundColourId, Colours::transparentWhite );
    sliceList.setMultipleSelectionEnabled(true);
    sliceList.setClickingTogglesRowSelection(true);
    addAndMakeVisible( sliceList );
}

SliceList::~SliceList()
{
    clearSlices();
}


void SliceList::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component


}


void SliceList::addSlice( Slice* slice )
{
    slices.add( slice );
    sliceList.updateContent();
    updateStates();
}

void SliceList::updateStates()
{
    for ( int i = 0; i < slices.size(); i++ )
    {
        if ( slices[i]->enabled )
            sliceList.selectRow( i, true, false );
        else
            sliceList.deselectRow( i );

    }
    sliceList.repaint();

}

void SliceList::clearSlices()
{
    slices.clear(false);
}

int SliceList::getNumRows()
{
    return slices.size();
}

void SliceList::listBoxItemClicked (int row, const MouseEvent&)
{
    slices[row]->enabled = !slices[row]->enabled;
    sendChangeMessage();
}

void SliceList::paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{


    Rectangle <int> rect ( width, height );
    rect.reduce( 1, 1 );
    
    if ( rowIsSelected )
    {
        g.setColour(Colours::darkblue);
        g.fillRect(rect);
        g.setColour(Colours::white);
    }
    
    else
        g.setColour(Colours::grey);
    String text = slices[rowNumber]->name;
    
    g.drawFittedText (text, 4 , 0, width - 4, height, Justification::centredLeft, true);
}

void SliceList::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    BorderSize<int> b = {5,5,5,5};
    sliceList.setBoundsInset(b);
    


}
