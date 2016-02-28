/*
  ==============================================================================

    Preview.cpp
    Created: 28 Dec 2014 11:00:00am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Preview.h"
#include "MainComponent.h"


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

void Preview::buttonClicked(Button* b)
{
	//create an int array of all the slices that are active
	Array<int> activeSlices;
	for ( int i = 0; i < sliceButtons.size(); i++ )
	{
		if ( sliceButtons[i]->getToggleState() )
			activeSlices.add(i);
	}
	
	MainContentComponent* parent = findParentComponentOfClass<MainContentComponent>();
	parent->chaseManager->setCurrentStep( activeSlices );
}

void Preview::setActiveSlices(Array<int> activeSlices)
{
    //turn all the slices off
    for ( int i = 0; i < sliceButtons.size(); i++ )
    {
        sliceButtons[i]->setToggleState( false, dontSendNotification );
    }
    
    //turn on the ones that should be on
    for ( int i = 0; i < activeSlices.size(); i++ )
    {
        //it could be that the xml we have loaded has more slices than the current button layout
        //for instance because it has been changed in Resolume
        //so here we make sure we aren't trying to change a buttonstate that no longer exists
        if ( activeSlices[i] < sliceButtons.size() )
            sliceButtons[activeSlices[i]]->setToggleState(true, dontSendNotification);
    }
	
	//so the slices get drawn again
	resized();
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

