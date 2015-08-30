/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
	laf = new ColourLookAndFeel();
    setLookAndFeel(laf);
	
	version = ProjectInfo::versionString;
	
    previewWindow = new Preview();
    addAndMakeVisible( previewWindow );
	previewWindow->addListener(this);
    
    sliceList = new SliceList();
    addAndMakeVisible(sliceList);
    sliceList->addChangeListener(this);
    
    sequencer = new Sequencer();
    addAndMakeVisible(sequencer);
    sequencer->addListener(this);
	
	copier = new Copier();
	addAndMakeVisible(copier);
	copier->addListener(this);
    
	//init the model vars
	currentSequence = 0;
	currentStep = 0;
	currentSequenceLength = 16;
	
	//create a sequence and set the oldest version it will correctly load
	xmlSequence = new XmlSequence( "0.0.1" );
	
	//load the slices from the xml if they exist
	Array<Slice> slices = xmlSequence->getSlices();
	for ( int i = 0; i < slices.size(); i++ )
	{
		Slice* s = new Slice ( slices[i] );
		previewWindow->addSlice(s);
		sliceList->addSlice( s );
	}
	
	//if we're loading a previously loaded assfile, set that as the activefile
	if ( xmlSequence->getFile() != String().empty )
		activeFile = xmlSequence->getFile();
	
	//update the view for the first step
	activeSlices = xmlSequence->getStep( currentSequence, currentStep );
	previewWindow->setSlices( activeSlices );
	sequencer->setSequenceNames ( xmlSequence->getSequenceNames() );
	sequencer->setSequenceLengths ( xmlSequence->getSequenceLengths() );
	currentSequenceLength = xmlSequence->getSequenceLengths()[ currentSequence ];

	//update the version number and save
	xmlSequence->setVersion( version );
	xmlSequence->save();
	
	//add a menu bar
	menuBar = new MenuBarComponent (this);
	addAndMakeVisible( menuBar );
	//setMacMainMenu(this);
	
    setSize (1280, 720);
}

MainContentComponent::~MainContentComponent()
{
    slices.clear();
    previewWindow = nullptr;
    sliceList = nullptr;
    xmlSequence = nullptr;
}

StringArray MainContentComponent::getMenuBarNames()
{
	const char* const names[] = { "File", nullptr };
	return StringArray (names);
}

PopupMenu MainContentComponent::getMenuForIndex(int menuIndex, const juce::String &menuName)
{
	PopupMenu menu;
	
	if (menuIndex == 0)
	{
		menu.addItem(1, "Load");
		menu.addItem(2, "Save");
		menu.addItem(3, "Save as...");
		menu.addSeparator();
		menu.addItem(4, "Load new ASS file");
	}

	return menu;
}

void MainContentComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	if ( topLevelMenuIndex == 0 )
	{
		switch ( menuItemID )
		{
			case 1:
				loadXml();
				break;
			case 2:
				saveXml();
				break;
			case 3:
				saveAsXml();
				break;
			case 4:
				loadAssFile();
				break;
			case 0:
			default:
				return;
		}
	}
}

void MainContentComponent::loadAssFile()
{
	File presetsLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 4/presets/screensetup/";
	FileChooser fc ( "Pick an ASS file...", presetsLocation, "*.xml", true );
	if ( fc.browseForFileToOpen() );
	{
		File f = fc.getResult();
		parseXml( f );
	}
}

void MainContentComponent::saveXml()
{
	xmlSequence->save();
}

void MainContentComponent::saveAsXml()
{
	//open a save dialog
}

void MainContentComponent::loadXml()
{
	//open a load dialog
}



void MainContentComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if ( source == sliceList )
    {
        for ( int i = 0; i < slices.size(); i++ )
        {
			xmlSequence->updateSlice (slices[i], i);
            previewWindow->update(slices[i], i );
        }
    }
}

void MainContentComponent::copierClicked(int multiplier)
{
	
	for ( int i = multiplier; i < currentSequenceLength; i += multiplier )
	{
		int nextStep = currentStep + i;
		if ( nextStep >= currentSequenceLength )
			nextStep -= currentSequenceLength;
		xmlSequence->setStep( currentSequence, nextStep, activeSlices );
		xmlSequence->save();
	}
}

void MainContentComponent::sliceClicked( Array<int> activeSlices_)
{
	activeSlices = activeSlices_;
	xmlSequence->setStep( currentSequence, currentStep, activeSlices );
	xmlSequence->save();
}

void MainContentComponent::stepSelected(int step)
{
	//the sequencer has been set to a new step or sequence
	//so read out the values for this step and update the preview
	currentStep = step;
	activeSlices = xmlSequence->getStep( currentSequence, currentStep );
	previewWindow->setSlices( activeSlices );
}

void MainContentComponent::sequenceNameChanged( String newName )
{
	//the sequence name has been changed
	//save the sequence name to xml
	xmlSequence->setSequenceName( currentSequence, newName );
	xmlSequence->save();
}

void MainContentComponent::sequenceSelected(int sequence)
{
	//when the sequence is changed
	//the first step is always triggered as well
	//so the preview state is updated by this click
	currentSequence = sequence;
}

void MainContentComponent::sequenceLengthChanged(int newSequenceLength)
{
	//this means the current sequence has been made shorter or longer
	//update the model var
	currentSequenceLength = newSequenceLength;
	
	//save it to xml
	xmlSequence->setNumberOfSteps( currentSequence, currentSequenceLength);
	xmlSequence->save();
	
}


void MainContentComponent::parseXml(File f)
{
    if ( !f.exists() )
    {
        DBG("Error loading file...");
        return;
    }
    else
    {
        DBG("Loading: " + f.getFullPathName() );
        XmlDocument xmlDoc ( f );
		
		//check if we're loading a new file or the same file
		//if it's new, clear the xml and create a fresh one
		bool loadingSameFile = (f == activeFile);
		if ( !loadingSameFile )
		{
			xmlSequence->createFreshXml( version );
		}

        ScopedPointer<XmlElement> preset (XmlDocument::parse ( f ));
        
        //traverse the whole fucking tree
        if (preset != nullptr && preset->hasTagName ("preset"))
        {
            //set the window name
            //getParentComponent()->setName(preset->getStringAttribute("name"));
            
            //clear the previewWindow
            slices.clear();
            
            forEachXmlChildElement( *preset, presetNode )
            {
                if ( presetNode != nullptr && presetNode->hasTagName("screen") )
                {
                    forEachXmlChildElement( *presetNode, screenNode )
                    {
                        if ( screenNode != nullptr && screenNode->hasTagName("slices") )
                        {
                            forEachXmlChildElement( *screenNode, slicesNode)
                            {
                                if ( slicesNode != nullptr && slicesNode->hasTagName("Slice") )
                                {
                                    String name;
                                    bool enabled = slicesNode->getBoolAttribute("isEnabled");
                                    int type;
                                    double l;
                                    double t;
                                    double r;
                                    double b;
                                    
                                    forEachXmlChildElement( *slicesNode, sliceNode)
                                    {
                                        if ( sliceNode != nullptr )
                                        {
                                            
                                            if ( sliceNode->hasTagName("name") )
                                            {
                                                name = sliceNode->getStringAttribute("value");
                                            }
                                            
                                            if ( sliceNode->hasTagName("type") )
                                            {
                                                type = sliceNode->getIntAttribute("value");
                                            }
                                            
                                            if ( sliceNode->hasTagName("rect") )
                                            {
                                                l = sliceNode->getDoubleAttribute( "l" );
                                                t = sliceNode->getDoubleAttribute( "t" );
                                                r = sliceNode->getDoubleAttribute( "r" );
                                                b = sliceNode->getDoubleAttribute( "b" );
                                            }
                                        }
                                    }
                                    
                                    //type 0 means only slices are loaded, no masks or crops
                                    if ( type == 0 )
                                    {
                                        Slice* slice = new Slice( name, enabled, l, t, r, b);
                                        slices.add( slice );
                                    }
                                }
                            }
                        }
                    }
                }
            }
			
			//if we're loading the same file, get the enabled states from the chaserxml
			if ( loadingSameFile )
			{
				Array<Slice> prevSlices = xmlSequence->getSlices() ;
				for ( int i = 0; i < prevSlices.size(); i++ )
				{
					if ( i < slices.size() )
						slices[i]->enabled = prevSlices[i].enabled;
				}
			}
			
            //update the previewWindow and sliceList
            sliceList->clearSlices();
            previewWindow->clearSlices();
            xmlSequence->clearSlices();
			
            for ( int i = 0; i < slices.size(); i++ )
            {
                sliceList->addSlice( slices[i] );
                previewWindow->addSlice( slices[i] );
                xmlSequence->addSlice ( slices[i] );
            }
			
            previewWindow->resized();

			//if we're not loading the same file, reset the sequencer
			if ( !loadingSameFile )
			{
				sequencer->setDefaultSequences();
			}
			
			//set the previewWindow to the correct step
			previewWindow->setSlices( xmlSequence->getStep( currentSequence, currentStep) );

			//store the last used file in xml and save it
            xmlSequence->setFile( f );
			xmlSequence->save();
			
			//update the activefile var
			activeFile = f;
        }
    }
}


void MainContentComponent::resized()
{

	Rectangle<int> area (getLocalBounds());
	
	float menuBarHeight = LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight();
	menuBar->setBounds (area.removeFromTop (menuBarHeight));
	
	AffineTransform scalePreview = AffineTransform::scale(0.83, (9.0 / 16.0) * 0.83 );
	Rectangle<int> previewArea = Rectangle<int>{0,0,getWidth(),getWidth()};
	previewArea = previewArea.transformedBy( scalePreview );
	previewArea.setPosition(0.0, menuBarHeight);
    previewWindow->setBounds(previewArea.reduced(5));
	
	Rectangle<int> sliceArea = Rectangle<int> { previewArea.getWidth(), int(menuBarHeight), area.getWidth() - previewArea.getWidth(), previewArea.getHeight()};
	sliceList->setBounds(sliceArea.reduced(5));

	Rectangle<int> bottomArea = area.removeFromBottom( area.getHeight() - previewArea.getHeight() );
	copier->setBounds(bottomArea.removeFromLeft(75).reduced(5));
	sequencer->setBounds(bottomArea.reduced(5));
	
}
