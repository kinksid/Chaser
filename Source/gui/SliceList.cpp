/*
  ==============================================================================

    SliceList.cpp
    Created: 10 Jan 2015 7:14:14pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceList.h"

MyPropertyPanel::MyPropertyPanel( SliceList& parent ) : parent( parent )
{
	setMessageWhenEmpty("(no screensetup loaded...)");
}

MyPropertyPanel::~MyPropertyPanel()
{
	
}

void MyPropertyPanel::resized()
{
	//first call the regular resized method
	PropertyPanel::resized();
	
	//now check the states of all the sections
	for ( int i = 0; i < getSectionNames().size(); i++ )
	{
		for ( int j = 0; j < parent.getSlicesFromSection(i).size(); j++)
		{
			//screen order is reversed in the list
			int currentScreen = getSectionNames().size() - 1 - i;
			parent.getSlicesFromSection( currentScreen )[j]->screenIsUncollapsed = isSectionOpen( i );
		}
	}
	
	parent.sliceVisibilityChanged();
}

SlicePropertyButton::SlicePropertyButton( SliceList& parent, Slice& slice ) : BooleanPropertyComponent(slice.name, slice.name, slice.name ), parent( parent ), slice(slice)
{
	state = slice.enabled;
}
SlicePropertyButton::~SlicePropertyButton(){}

void SlicePropertyButton::buttonClicked ( Button* b )
{
	setState( !getState() );
	b->setToggleState( getState(), dontSendNotification );
	slice.enabled = getState();
	
	parent.sliceVisibilityChanged();
}

void SlicePropertyButton::setState (bool newState)
{
	state = newState;
}

bool SlicePropertyButton::getState() const
{
	return state;
}

Slice& SlicePropertyButton::getSlice()
{
	return slice;
}





//==============================================================================
SliceList::SliceList() : panel( new MyPropertyPanel(*this ))
{
	addAndMakeVisible( panel );
}

SliceList::~SliceList()
{

}

Array<Slice*> SliceList::getSlicesFromSection(int i)
{
	Array<Slice*> returnArray;
	
	for ( int j = 0; j < sections[i]->sliceToggles.size(); j++ )
	{
		returnArray.add ( &static_cast<SlicePropertyButton*>(sections[i]->sliceToggles[j])->getSlice() );
	}
	
	return returnArray;
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
				SlicePropertyButton* newComponent = new SlicePropertyButton( *this, *slices[i] );
				newComponent->setColour(BooleanPropertyComponent::backgroundColourId, claf.backgroundColour);
				newComponent->setState( slices[i]->enabled );
				sections[j]->sliceToggles.add( newComponent );
			}
		}
	}
	
	//now go through all the arrays again and create sections out of them
	for ( int i = sections.size() - 1; i >= 0 ; i-- )
		panel->addSection( sections[i]->screenPair.second, sections[i]->sliceToggles );
	
	
}


void SliceList::sliceVisibilityChanged()
{
	getParentComponent()->resized();
}

void SliceList::clear()
{
	uniqueIds.clear();
	sections.clear();
	panel->clear();
}

void SliceList::resized()
{
    BorderSize<int> b = {5,5,5,5};
	panel->setBoundsInset(b);
}
