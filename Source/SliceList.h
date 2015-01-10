/*
  ==============================================================================

    SliceList.h
    Created: 10 Jan 2015 7:14:14pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef SLICELIST_H_INCLUDED
#define SLICELIST_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Slice.h"

//==============================================================================
/*
*/
class SliceList    : public Component, public ListBoxModel, public ChangeBroadcaster
{
public:
    SliceList();
    ~SliceList();

    int getNumRows();
    virtual void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
    virtual void listBoxItemClicked (int row, const MouseEvent&);
    
    void paint (Graphics&);
    void resized();
    
    void addSlice( Slice* slice );
    void clearSlices();
    void updateStates();

private:
    OwnedArray<Slice> slices;
    ListBox sliceList;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceList)
};


#endif  // SLICELIST_H_INCLUDED
