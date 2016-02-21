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
//#include "XmlSequence.h"
#include "Copier.h"
#include "ColourLookAndFeel.h"
#include "ChaseManager.h"
#include "SliceManager.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
								public MenuBarModel,
								public Sequencer::Listener,
								public Preview::Listener,
								public Copier::Listener,
								public KeyListener,
								public Timer

{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized() override;

	virtual void timerCallback() override;
	
	//menubar methods
	virtual StringArray getMenuBarNames() override;
	virtual PopupMenu getMenuForIndex (int topLevelMenuIndex,
									   const String& menuName) override;
	virtual void menuItemSelected (int menuItemID,
								   int topLevelMenuIndex) override;
	
	//sequencer listener methods
	virtual void stepSelected ( int step ) override;
	virtual void sequenceNameChanged ( String newName ) override;
	virtual void sequenceSelected ( int sequence ) override;
	virtual void sequenceLengthChanged ( int newSequenceLength ) override;
	
	//preview listener methods
	virtual void sliceClicked ( Array<int> activeSlices ) override;
	
	//copier listener methods
	virtual void copierClicked ( int m ) override;
	
	//keyboard listener
	virtual bool keyPressed (const KeyPress& key,
							 Component* originatingComponent) override;

	
	
	void saveXml();
	bool saveAsXml();
	void loadXml();
    
	void reloadSliceData();
	void clearGUI();
	

	
	void copyStep();
	void pasteStep();
	void copySequence();
	void pasteSequence();
	
	ChaseManager& chaseManager;
	SliceManager& sliceManager;
	
private:
	ScopedPointer<MenuBarComponent> menuBar;
	
	int currentStep;
	int currentSequence;
	int currentSequenceLength;
	Array<int> activeSlices;
	Array<int> slicesToCopy;
	Array<Array<int>> currentSequenceSlices;
	String sequenceNameToCopy;
	
    ScopedPointer<ColourLookAndFeel> laf;
    
    OwnedArray<Slice> slices;
    
    ScopedPointer<Preview> previewWindow;
    ScopedPointer<SliceList> sliceList;
    ScopedPointer<Sequencer> sequencer;
	// ScopedPointer<XmlSequence> xmlSequence;
	ScopedPointer<Copier> copier;
	

	


	String version;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
