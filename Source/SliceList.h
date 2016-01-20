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
#include "ColourLookAndFeel.h"

//==============================================================================
/*
*/

/*
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
	ColourLookAndFeel claf;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceList)
};
 */



class SlicePropertyButton : public BooleanPropertyComponent
{
public:
	SlicePropertyButton(String n) : BooleanPropertyComponent(n, n, n )
	{
		state = false;
	}
	~SlicePropertyButton(){}
	
	void buttonClicked ( Button* b ) override
	{
		setState( !getState() );
		b->setToggleState( getState(), dontSendNotification );
	}
	
	void setState (bool newState) override
	{
		state = newState;
	}
	
	bool getState() const override
	{
		return state;
	}
	
	
private:
	
	bool state;
	
	
};

class SliceList : public Component
{
public:
	SliceList();
	~SliceList();
	
	void paint(Graphics&) override;
	void resized() override;
	
	void addSlices( OwnedArray<Slice>& slices );
	void clear();
	void updateStates();
private:
	struct NamedArray
	{
	public:
		NamedArray() {}
		~NamedArray() {}
		
		String name;
		int uniqueId;
		Array<PropertyComponent*> array;
		
	};
	Array<Slice*> slices;
	ColourLookAndFeel claf;
	Array<int> uniqueIds;
	OwnedArray<NamedArray> sections;
	
	PropertyPanel panel;
	
	
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceList)
	
};


#endif  // SLICELIST_H_INCLUDED
