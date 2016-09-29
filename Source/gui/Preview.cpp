/*
  ==============================================================================

    Preview.cpp
    Created: 28 Dec 2014 11:00:00am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Preview.h"
#include "../MainComponent.h"


//==============================================================================
Preview::Preview()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    sliceLaf = new SliceLookAndFeel();
	sliceButtons.clear();
}

Preview::~Preview()
{
    clearSlices();
}

void Preview::buttonClicked(Button*)
{
	//using uid instead of index should make sure that once we change the state again
	//1. non-existent slices in a step get removed because their id is no longer in the list
	//2. slices that had their index changed still get chased correctly

	//create an int array of all the slices that are active
	Array<int64> activeSlices;
	for ( int i = 0; i < sliceButtons.size(); i++ )
	{
		if ( sliceButtons[ i ]->getToggleState() )
		{
			int64 uid = sliceButtons[ i ]->getUniqueId();
			activeSlices.add( uid );
		}
	}
	
	MainContentComponent* parent = findParentComponentOfClass<MainContentComponent>();
	parent->chaseManager->setCurrentStep( activeSlices );
}

void Preview::setActiveSlices(Array<int64> activeSliceIds)
{
    //turn all the slices off
    for ( int i = 0; i < sliceButtons.size(); i++ )
    {
        sliceButtons[i]->setToggleState( false, dontSendNotification );
    }
    
    //turn on the ones that should be on
	
	for ( int i = 0; i < activeSliceIds.size(); i++ )
    {
		//go through all the slicebuttons and see if their uid matches
		for ( SliceButton* button : sliceButtons )
		{
			if ( activeSliceIds[ i ] == button->getUniqueId() )
				button->setToggleState( true, dontSendNotification );

			button->repaint();
		}
    }
}

void Preview::createSliceButtons(OwnedArray<Slice> &slices)
{
	for ( int i = 0; i < slices.size(); i++ )
		addSliceButton( *slices[i] );
}

void Preview::addSliceButton( Slice& slice )
{
	SliceButton* newButton = new SliceButton ( slice, Point<int> (getWidth(), getHeight()) );
    newButton->setLookAndFeel(sliceLaf);
	newButton->setColours(sliceLaf->backgroundColour, sliceLaf->textColour, sliceLaf->primaryColour);
    newButton->addListener(this);

    sliceButtons.add( newButton );
	
    if ( newButton->isVisible() )
        addAndMakeVisible( newButton );
    else
        addChildComponent( newButton );
	
	//flip the order, so it matches the order in Resolume
	newButton->toBack();
}

void Preview::clearSlices()
{
    sliceButtons.clear();
}

void Preview::paint (Graphics& g)
{
    g.fillAll (Colours::black);   // clear the background
    
    g.setColour (sliceLaf->outlineColour);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    g.setColour (sliceLaf->outlineColour);
    g.setFont (14.0f);
    g.drawText ("Preview", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Preview::resized()
{
    for ( int i = 0; i < sliceButtons.size() ; i++ )
		sliceButtons[i]->resized();
}

