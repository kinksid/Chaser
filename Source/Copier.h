/*
  ==============================================================================

    Copier.h
    Created: 1 Feb 2015 3:36:41pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef COPIER_H_INCLUDED
#define COPIER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 
 This class is used to copy the current step to other steps
*/
class Copier    : public Component, public Button::Listener
{
public:
    Copier();
    ~Copier();
	
	//listener functions
	virtual void buttonClicked (Button*);

	//component functions
    void paint (Graphics&);
    void resized();
	
	class Listener
	{
		
	public:
		virtual ~Listener() {}
		virtual void copierClicked ( int m ) = 0;
	};
	
	void addListener ( Listener* const l ) { listeners.add ( l ); }
	void removeListener ( Listener* const l ) { listeners.remove (l ); }

private:
	OwnedArray<Button> buttons;
	
	ListenerList<Listener> listeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Copier)
};


#endif  // COPIER_H_INCLUDED
