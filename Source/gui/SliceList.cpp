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
	addAndMakeVisible( panel );
}

SliceList::~SliceList()
{

}


void SliceList::paint (Graphics& g)
{
    g.fillAll (claf.backgroundColour);   // clear the background

    g.setColour (claf.outlineColour);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}



void SliceList::addSlices( OwnedArray<Slice>& slices )
{
	clear();
	
	for ( int i = 0; i < slices.size(); i++ )
	{
		int id = slices[i]->screenPair.first;
		
		//make a new section for each uniqued id
		if ( !uniqueIds.contains( id ))
		{
			uniqueIds.add(id);
			NamedArray* newSection = new NamedArray();
			DBG(slices[i]->screenPair.second);
			newSection->screenPair = slices[i]->screenPair;
			sections.add ( newSection );
		}
	}
	
	//now go through the slices again
	//if their unique id matches the sections
	//add it to the array for that section
	for ( int i = 0; i < slices.size(); i++ )
	{
		int id = slices[i]->screenPair.first;
		
		for ( int j = 0; j < sections.size(); j++ )
		{
			if ( id == sections[j]->screenPair.first )
			{
				//create a new SlicePropertyButton
				SlicePropertyButton* newComponent = new SlicePropertyButton( slices[i] );
				newComponent->addListener(this);
				newComponent->setState( slices[i]->enabled );
				sections[j]->array.add( newComponent );
			}
		}
	}
	
	//now go through all the arrays again and create sections out of them
	for ( int i = sections.size() - 1; i >= 0 ; i-- )
		panel.addSection( sections[i]->screenPair.second, sections[i]->array );
	
	resized();
}

void SliceList::sliceToggled()
{
	Component::BailOutChecker checker (this);
	if (! checker.shouldBailOut())
		listeners.callChecked ( checker, &SliceList::Listener::sliceVisibilityChanged );

}

void SliceList::clear()
{
	uniqueIds.clear();
	sections.clear();
	panel.clear();
}

void SliceList::resized()
{
    BorderSize<int> b = {5,5,5,5};
	panel.setBoundsInset(b);
}
