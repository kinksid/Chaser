/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Preview.h"
#include "Slice.h"
#include "SliceList.h"
#include "Sequencer.h"
#include "XmlSequence.h"
#include "Copier.h"
#include "ColourLookAndFeel.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
								public MenuBarModel,
								public ChangeListener,
								public Sequencer::Listener,
								public Preview::Listener,
								public Copier::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized();
	
    virtual void changeListenerCallback (ChangeBroadcaster* source);
	
	//menubar methods
	virtual StringArray getMenuBarNames();
	virtual PopupMenu getMenuForIndex (int topLevelMenuIndex,
									   const String& menuName);
	virtual void menuItemSelected (int menuItemID,
								   int topLevelMenuIndex);
	
	//sequencer listener methods
	virtual void stepSelected ( int step );
	virtual void sequenceNameChanged ( String newName );
	virtual void sequenceSelected ( int sequence );
	virtual void sequenceLengthChanged ( int newSequenceLength );
	
	//preview listener methods
	virtual void sliceClicked ( Array<int> activeSlices );
	
	//copier listener methods
	virtual void copierClicked ( int m );
	
	void loadAssFile();
	void saveXml();
	void saveAsXml();
	void loadXml();
    void parseXml ( File f );

	

private:
	ScopedPointer<MenuBarComponent> menuBar;
	
	int currentStep;
	int currentSequence;
	int currentSequenceLength;
	Array<int> activeSlices;
	
    ScopedPointer<ColourLookAndFeel> laf;
    
    File activeFile;
    
    OwnedArray<Slice> slices;
    
    ScopedPointer<Preview> previewWindow;
    ScopedPointer<SliceList> sliceList;
    ScopedPointer<Sequencer> sequencer;
    ScopedPointer<XmlSequence> xmlSequence;
	ScopedPointer<Copier> copier;
	
	String version;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
