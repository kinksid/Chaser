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
								public Copier::Listener,
								public TextEditor::Listener,
								public Timer

{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized() override;
	
    virtual void changeListenerCallback (ChangeBroadcaster* source) override;
	
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
	
	//texteditor listener methods
	virtual void textEditorReturnKeyPressed (TextEditor&) override;
	virtual void textEditorEscapeKeyPressed (TextEditor&) override;
	virtual void textEditorFocusLost (TextEditor&) override;
	
	void loadAssFile();
	void reloadAssFile();
	void saveXml();
	void saveAsXml();
	void loadXml();
    void parseXml ( File f );
	void reloadSliceData();

	

private:
	ScopedPointer<MenuBarComponent> menuBar;
	
	int currentStep;
	int currentSequence;
	int currentSequenceLength;
	Array<int> activeSlices;
	
    ScopedPointer<ColourLookAndFeel> laf;
    
    OwnedArray<Slice> slices;
    
    ScopedPointer<Preview> previewWindow;
    ScopedPointer<SliceList> sliceList;
    ScopedPointer<Sequencer> sequencer;
    ScopedPointer<XmlSequence> xmlSequence;
	ScopedPointer<Copier> copier;
	
	ScopedPointer<TextEditor> compWidth;
	ScopedPointer<TextEditor> compHeight;
	Point<int> resolution;
	
	String version;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
