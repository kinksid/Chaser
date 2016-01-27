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
    /*
    sliceList.setModel( this );
    sliceList.setColour(ListBox::backgroundColourId, Colours::transparentWhite );
    sliceList.setMultipleSelectionEnabled(true);
    sliceList.setClickingTogglesRowSelection(true);
    addAndMakeVisible( sliceList );
	 */
	
	addAndMakeVisible( panel );
}

SliceList::~SliceList()
{
	//clearSlices();
}


void SliceList::paint (Graphics& g)
{
	
    g.fillAll (claf.backgroundColour);   // clear the background

    g.setColour (claf.outlineColour);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component


}



void SliceList::addSlices( OwnedArray<Slice>& slices )
{
	//slices.add( slice );
	//sliceList.updateContent();
	//slices.addArray( slices_, slices_.size());
	
	
	
	clear();
	
	
	for ( int i = 0; i < slices.size(); i++ )
	{
		int id = slices[i]->uniqueId;
		
		//make a new section for each uniqued id
		if ( !uniqueIds.contains( id ))
		{
			uniqueIds.add(id);
			NamedArray* newSection = new NamedArray();
			DBG(slices[i]->screen);
			newSection->name = slices[i]->screen;
			newSection->uniqueId = id;
			sections.add ( newSection );
		}
	}
	
	//now go through the slices again
	//if their unique id matches the sections
	//add it to the array for that section
	for ( int i = 0; i < slices.size(); i++ )
	{
		int id = slices[i]->uniqueId;
		
		for ( int j = 0; j < sections.size(); j++ )
		{
			if ( id == sections[j]->uniqueId )
			{
				//create a new BooleanPropery
				String name = slices[i]->name;
				SlicePropertyButton* newComponent = new SlicePropertyButton( slices[i] );
				newComponent->addListener(this);
				newComponent->setState( slices[i]->enabled );
				sections[j]->array.add( newComponent );
			}
		}
	}
	
	//now go through all the arrays again and create sections out of them
	for ( int i = sections.size() - 1; i >= 0 ; i-- )
		panel.addSection( sections[i]->name, sections[i]->array );
	
    updateStates();
}

void SliceList::sliceToggled()
{

	Component::BailOutChecker checker (this);
	if (! checker.shouldBailOut())
		listeners.callChecked ( checker, &SliceList::Listener::sliceVisibilityChanged );

}

void SliceList::updateStates()
{
    for ( int i = 0; i < slices.size(); i++ )
    {
		/*
        if ( slices[i]->enabled )
            sliceList.selectRow( i, true, false );
        else
            sliceList.deselectRow( i );
		 */

    }
	//  sliceList.repaint();
	resized();
}


void SliceList::clear()
{
	uniqueIds.clear();
	sections.clear();
	panel.clear();
}


/*
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
		
        g.setColour(claf.primaryColour);
        g.fillRect(rect);
        g.setColour(claf.textOnColour);
    }
    
    else
        g.setColour(claf.textColour);
	
	if (rowNumber < slices.size())
	{
		String text = slices[rowNumber]->name;
		g.drawFittedText(text, 4, 0, width - 4, height, Justification::centredLeft, true);
	}
}
 */

void SliceList::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    BorderSize<int> b = {5,5,5,5};
	panel.setBoundsInset(b);
    


}
