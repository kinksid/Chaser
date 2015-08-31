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
	
	//add a menu bar
	menuBar = new MenuBarComponent (this);
	addAndMakeVisible( menuBar );
	//setMacMainMenu(this);
    
	//init the model vars
	currentSequence = 0;
	currentStep = 0;
	currentSequenceLength = 16;
	
	//create a sequence and set the oldest version it will correctly load
	xmlSequence = new XmlSequence( "0.0.1" );
	
	//check if we had a file previously loaded
	//if so, load that bad boy
	//if not, ask to load a new one
	if (xmlSequence->getXmlFile() != File())
	{
		xmlSequence->loadXmlFile(xmlSequence->getXmlFile());
		//if something goes wrong here, fuck it
	}
	else
		xmlSequence->createFreshXml( version );

	reloadSliceData();
	
    setSize (1280, 720);
	
	//start a timer to update the window name
	startTimer( 1000 );
}

MainContentComponent::~MainContentComponent()
{
    slices.clear();
    previewWindow = nullptr;
    sliceList = nullptr;
    xmlSequence = nullptr;
}

void MainContentComponent::timerCallback()
{
	//set the name
	getTopLevelComponent()->setName( xmlSequence->getXmlFile().getFileNameWithoutExtension());
	
	stopTimer();
}

void MainContentComponent::reloadSliceData()
{
	sliceList->clearSlices();
	previewWindow->clearSlices();
	slices.clear();
	
	//load the slices from the xml if they exist
	Array<Slice> slicesInXml = xmlSequence->getSlices();
	for ( int i = 0; i < slicesInXml.size(); i++ )
	{
		Slice* s = new Slice ( slicesInXml[i] );
		previewWindow->addSlice (s) ;
		sliceList->addSlice( s );
		slices.add(s);
	}
	
	//update the view for the first step
	activeSlices = xmlSequence->getStep( currentSequence, currentStep );
	previewWindow->setSlices( activeSlices );
	sequencer->setSequenceNames ( xmlSequence->getSequenceNames() );
	sequencer->setSequenceLengths ( xmlSequence->getSequenceLengths() );
	currentSequenceLength = xmlSequence->getSequenceLengths()[ currentSequence ];
	
	resized();
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
		menu.addItem(1, "New");
		menu.addItem(2, "Load");
		menu.addItem(3, "Save");
		menu.addItem(4, "Save as...");
		menu.addSeparator();
		menu.addItem(5, "Reload ASS file");
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
				xmlSequence->createFreshXml(version);
				saveAsXml();
				loadAssFile();
				break;
			case 2:
				loadXml();
				break;
			case 3:
				saveXml();
				break;
			case 4:
				saveAsXml();
				break;
			case 5:
				reloadAssFile();
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
	if ( fc.browseForFileToOpen() )
	{
		File f = fc.getResult();
		parseXml( f );
	}
}

void MainContentComponent::reloadAssFile()
{
	parseXml( xmlSequence->getAssFile() );
}

void MainContentComponent::saveXml()
{
	xmlSequence->save();
}

void MainContentComponent::saveAsXml()
{
	//open a save dialog
	File chaserLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Chaser/";
	if (!chaserLocation.exists())
		chaserLocation.createDirectory();
	FileChooser fc ( "Save chaser as...", chaserLocation, "*.xml", true );
	if ( fc.browseForFileToSave(true) )
	{
		File f = fc.getResult();
		
		xmlSequence->setXmlFile( f );
		xmlSequence->save();
		
		getTopLevelComponent()->setName(f.getFileNameWithoutExtension());
	}
}

void MainContentComponent::loadXml()
{
	//open a load dialog
	File chaserLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Chaser/";
	FileChooser fc ( "Pick a chaser file...", chaserLocation, "*.xml", true );
	
	if ( fc.browseForFileToOpen() )
	{
		File f = fc.getResult();
	
		if ( !xmlSequence->loadXmlFile( f ))
		{
			AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
											  "Sorry!",
											  "Couldn't load that file.",
											  "Ok");
		}
		else
		{
			reloadSliceData();
			getTopLevelComponent()->setName(f.getFileNameWithoutExtension());
		}
	}
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
		
		saveXml();
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
		AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
										  "Sorry!",
										  "Couldn't reload that file. It looks like it's gone.",
										  "Ok");
        DBG("Error loading file...");
        return;
    }
    else
    {
        DBG("Loading: " + f.getFullPathName() );
        XmlDocument xmlDoc ( f );
		
		//check if we're loading a new file or the same file
		//if it's new, clear the xml and create a fresh one
		bool loadingSameFile = (f == xmlSequence->getAssFile() );
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

			//store the last used ass file in xml and save it
            xmlSequence->setAssFile( f );
			xmlSequence->save();
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
