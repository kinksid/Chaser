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


class SlicePropertyButton : public BooleanPropertyComponent
{
public:
	SlicePropertyButton(Slice* slice) : BooleanPropertyComponent(slice->name, slice->name, slice->name ), slice(slice)
	{
		state = slice->enabled;
	}
	~SlicePropertyButton(){}
	
	void buttonClicked ( Button* b ) override
	{
		setState( !getState() );
		b->setToggleState( getState(), dontSendNotification );
		slice->enabled = getState();
		
		Component::BailOutChecker checker (this);
		if (! checker.shouldBailOut())
			listeners.callChecked ( checker, &SlicePropertyButton::Listener::sliceToggled );
	}
	
	void setState (bool newState) override
	{
		state = newState;
	}
	
	bool getState() const override
	{
		return state;
	}
	
	class Listener
	{
		
	public:
		virtual ~Listener() {}
		virtual void sliceToggled () = 0;
		
	};
	
	void addListener ( Listener* const l ) { listeners.add ( l ); }
	void removeListener ( Listener* const l ) { listeners.remove (l ); }
	
	
private:
	Slice* slice;
	bool state;
	
	ListenerList<Listener> listeners;
	
};

class SliceList : public Component, public SlicePropertyButton::Listener
{
public:
	SliceList();
	~SliceList();
	
	void paint(Graphics&) override;
	void resized() override;
	
	void addSlices( OwnedArray<Slice>& slices );
	void clear();
	//void updateStates();
	
	class Listener
	{
		
	public:
		virtual ~Listener() {}
		virtual void sliceVisibilityChanged () = 0;
		
	};
	
	void addListener ( Listener* const l ) { listeners.add ( l ); }
	void removeListener ( Listener* const l ) { listeners.remove (l ); }
	
	virtual void sliceToggled () override;
	

	
private:
	struct NamedArray
	{
	public:
		NamedArray() {}
		~NamedArray() {}
		
		std::pair<int, String> screenPair;
		Array<PropertyComponent*> array;
		
	};
	Array<Slice*> slices;
	ColourLookAndFeel claf;
	Array<int> uniqueIds;
	OwnedArray<NamedArray> sections;
	
	PropertyPanel panel;
	
	ListenerList<Listener> listeners;
	
	
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceList)
	
};


#endif  // SLICELIST_H_INCLUDED
