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


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public Button::Listener, public ChangeListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized();
    void buttonClicked( Button* b );
    
    virtual void changeListenerCallback (ChangeBroadcaster* source);
    
    void parseXml ( File f );
    
    

private:
    
    ScopedPointer<LookAndFeel> laf;
    
    ScopedPointer<Button> openOutput;
    File activeFile;
    
    OwnedArray<Slice> slices;
    
    ScopedPointer<Preview> previewWindow;
    ScopedPointer<SliceList> sliceList;
    ScopedPointer<Sequencer> sequencer;
    ScopedPointer<XmlSequence> xmlSequence;
    
    //ScopedPointer<Button> update;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
