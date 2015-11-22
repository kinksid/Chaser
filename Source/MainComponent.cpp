/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "XmlParser.h"


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
	
	//create a sequence and set the oldest version it will correctly load
	xmlSequence = new XmlSequence( "0.0.1" );

	//add a menu bar
	menuBar = new MenuBarComponent (this);
#if JUCE_WINDOWS
	addAndMakeVisible( menuBar );
#elif JUCE_MAC
	setMacMainMenu(this);
#endif
    
	//init the model vars
	currentSequence = 0;
	currentStep = 0;
	currentSequenceLength = 16;
	
	addKeyListener(this);
	
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
	#if JUCE_MAC
	setMacMainMenu(nullptr);
	#endif
}

void MainContentComponent::timerCallback()
{
	
	
	
	//check if we had a file previously loaded
	//if so, load that bad boy
	//if not, load a new one called DefaultXml
	File savedFile = xmlSequence->getXmlFileFromPreferences();
	if (savedFile != File() && savedFile.exists() )
	{
		xmlSequence->loadXmlFile(savedFile);
		//this will return 1920x1080 if no resolution was saved
		resolution = xmlSequence->getResolution();
		reloadSliceData();
		//if something goes wrong here, fuck it
	}
	else
	{
		xmlSequence->createFreshXml( version );
		resolution = xmlSequence->getResolution();
		saveXml();
		//try to load the default
		loadDefaultAssFile();
	}
	//set the name
	getTopLevelComponent()->setName( xmlSequence->getXmlFile().getFileNameWithoutExtension());
	
	//resize to update preview window
	resized();
	
	stopTimer();
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
		menu.addItem(1, "Load Arena Setup");
		menu.addItem(2, "Reload Arena Setup", xmlSequence->getAssFile() == File()? false: true );
		menu.addSeparator();
		menu.addItem(3, "New Chaser");
		menu.addSeparator();
		menu.addItem(4, "Load Chaser");
		//menu.addItem(5, "Save Chaser");
		menu.addItem(5, "Save Chaser as...");
	
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
				resolution = xmlSequence->getResolution();
				loadAssFile();
				saveXml();
				break;
			case 2:
				reloadAssFile();
				break;
			case 3:
			{
				//create a fresh xml file
				xmlSequence->createFreshXml(version);
				//set the filename to DefaultChaserxml
				File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
				File defaultChaseFile = docDir.getChildFile("Chaser/DefaultChaser.xml");
				xmlSequence->setXmlFile( defaultChaseFile );
				getTopLevelComponent()->setName(defaultChaseFile.getFileNameWithoutExtension());
				//read out the resolution
				resolution = xmlSequence->getResolution();
				//this will make sure nothing is loaded and everything is empty
				clearGUI();
				//try to load the default assfile
				loadDefaultAssFile();
				break;
			}
			case 4:
				loadXml();
				break;
			case 5:
				saveAsXml();
				break;
				/*
			case 6:
				saveAsXml();
				break;
				 */

			case 0:
			default:
				return;
		}
	}
}

void MainContentComponent::clearGUI()
{
	//clear the previewWindow and sliceList
	//empty the xmlSequence
	//reset the sequencer
	
	sliceList->clearSlices();
	previewWindow->clearSlices();
	xmlSequence->clearSlices();
	sequencer->setDefaultSequences();
	resized();
}


void MainContentComponent::loadAssFile()
{
	//first check 5, then check 4
	File presetsLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 5/presets/screensetup/";
	if ( !presetsLocation.exists() )
		presetsLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 4/presets/screensetup/";
	
	FileChooser fc ( "Pick an Arena setup file...", presetsLocation, "*.xml", true );
	if ( fc.browseForFileToOpen() )
	{
		File f = fc.getResult();
		if (!parseAssXml( f ) )
			throwLoadError();
	}
}

void MainContentComponent::loadDefaultAssFile()
{
	//check for the Arena 5 preset
	File advancedLocation;
	
	advancedLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 5/preferences/screensetup/advanced.xml";
	if ( advancedLocation.exists() )
	{
		if ( AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::QuestionIcon,
										 "Res 5 setup file found!",
										 "Would you like to load the current Arena setup file?",
										 "OK", "Cancel") )
		{
			DBG("Trying to load Arena 5 file...");
			
			//try to get the file name of the xml that is currently loaded
			ScopedPointer<XmlElement> xmlRoot (XmlDocument::parse ( advancedLocation ));
			String advancedName = XmlParser::getAdvancedPresetNameFromRes5Xml( *xmlRoot );
			
			//if it's got a name, parse and load that file
			if ( advancedName != String().empty )
			{
				File preset = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 5/presets/screensetup/" + advancedName + ".xml";
				if (!parseAssXml( preset ))
					throwLoadError();
			}
			else
			{
				//try to load the information in the advanced file itself
				if(!parseAssXml( advancedLocation ))
					throwLoadError();
			}
		}
	}
	//check for the Arena 4 preset
	else
	{
		advancedLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 4/preferences/config.xml";
		if ( advancedLocation.exists() )
		{
		
			if ( AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::QuestionIcon,
											  "Res 4 setup file found!",
											  "Would you like to load the current Arena setup file?",
											  "OK", "Cancel") )
			{
				DBG("Trying to load Arena 4 file...");
				//this requires parsing of the config xml
				if ( !parseAssXml( advancedLocation ))
					throwLoadError();
			}
		}
		
	}
	
	
}

void MainContentComponent::reloadAssFile()
{
	if (!parseAssXml( xmlSequence->getAssFile() ))
		throwLoadError();
}

void MainContentComponent::saveXml()
{
	if (!xmlSequence->save())
	{
		DBG("SAVE ERROR!");
		throwSaveError();
	}
}

bool MainContentComponent::saveAsXml()
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
		saveXml();
		
		getTopLevelComponent()->setName(f.getFileNameWithoutExtension());
		return true;
	}
	return false;
}

void MainContentComponent::loadXml()
{
	//open a load dialog
	File chaserLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Chaser/";
	FileChooser fc ( "Pick a Chaser file...", chaserLocation, "*.xml", true );
	
	if ( fc.browseForFileToOpen() )
	{
		File f = fc.getResult();
	
		if ( !xmlSequence->loadXmlFile( f ))
		{
			throwLoadError();
		}
		else
		{
			reloadSliceData();
			getTopLevelComponent()->setName(f.getFileNameWithoutExtension());
			resized();
		}
	}
}


bool MainContentComponent::parseAssXml(File f)
{
	if ( !f.exists() )
	{
		return false;
	}
	else
	{
		//check if we're loading a new file or the same file
		//if it's new, clear the xml and create a fresh one
		bool loadingSameFile = (f == xmlSequence->getAssFile() );
		if ( !loadingSameFile )
		{
			xmlSequence->createFreshXml( version );
		}
		
		DBG("Loading: " + f.getFullPathName() );
	
		ScopedPointer<XmlElement> xmlRoot;
		xmlRoot = (XmlDocument::parse ( f ));
		
		//see if we're dealing with a res 4 or res 5 ass file
		//then try to parse it
		bool succesfulParse = false;
		
		if (xmlRoot != nullptr && xmlRoot->hasTagName ("preset"))
			succesfulParse = XmlParser::parseRes4Xml( *xmlRoot, slices, resolution );
		else if ( xmlRoot != nullptr && xmlRoot->hasTagName("XmlState"))
			succesfulParse = XmlParser::parseRes5Xml( *xmlRoot, slices, resolution );
		else if ( xmlRoot != nullptr && xmlRoot->hasTagName("ScreenSetup") )
			succesfulParse = XmlParser::parseRes5PrefXml( *xmlRoot, slices, resolution);
		else if ( xmlRoot != nullptr && xmlRoot->hasTagName("avenue"))
			succesfulParse = XmlParser::parseRes4ConfigXml( *xmlRoot, slices, resolution);

		
		//if the file was susccesfully parsed
		if ( succesfulParse )
		{
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
			
			//set the resolution in the xml
			xmlSequence->setResolution( resolution );
			
			//store the last used ass file in xml and save it
			xmlSequence->setAssFile( f );
			saveXml();
			
			//update the numbers
			resized();
			
			return true;
		}
		
		else
		{
			return false;
		}
	}
	
	return false;
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
	
	resolution = xmlSequence->getResolution();
	resized();
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
		saveXml();
	}
}

void MainContentComponent::sliceClicked( Array<int> activeSlices_)
{
	activeSlices = activeSlices_;
	xmlSequence->setStep( currentSequence, currentStep, activeSlices );
	saveXml();
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
	saveXml();
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
	if ( xmlSequence->getSequenceLengths().size() > currentSequence )
	{
		xmlSequence->setNumberOfSteps( currentSequence, currentSequenceLength);
		saveXml();
	}
	
}



bool MainContentComponent::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent)
{
	
	if ( key == KeyPress::leftKey )
	{
		sequencer->previousStep();
	}
	else if ( key == KeyPress::rightKey )
	{
		sequencer->nextStep();
	}
		
	return true;
}

void MainContentComponent::throwLoadError()
{
	AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
									 "Sorry!",
									 "Something went wrong reading that file.",
									 "Ok");
	DBG("Error loading file...");
}

void MainContentComponent::throwSaveError()
{
	AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
									  "Sorry!",
									  "Could not save data.",
									  "Ok");
}

void MainContentComponent::resized()
{

	Rectangle<int> area (getLocalBounds());
	
	int menuBarHeight = 0;
	#if JUCE_WINDOWS
	menuBarHeight = LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight();
	#elif JUCE_MAC
	menuBarHeight = 0;
	#endif
	
	menuBar->setBounds (area.removeFromTop (menuBarHeight));

	Rectangle<int> previewArea = Rectangle<int>{0, 0, int(getWidth() * 0.83), int((getWidth() / 16.0) * 9.0 * 0.83) };
	previewArea.setPosition(0,  menuBarHeight);
	
	previewWindow->setBounds(0,0,resolution.x, resolution.y);
	Rectangle<int> previewWindowArea = previewArea.reduced(5);

	if(previewWindow->getWidth() > 0 && previewWindow->getHeight() > 0 )
		previewWindow->setBoundsToFit( previewWindowArea.getX(), previewWindowArea.getY(), previewWindowArea.getWidth(), previewWindowArea.getHeight(), Justification::centred, false);
	
	Rectangle<int> sliceArea = Rectangle<int> { previewArea.getWidth(), menuBarHeight, area.getWidth() - previewArea.getWidth(), previewArea.getHeight()};
	sliceList->setBounds(sliceArea.reduced(5));

	Rectangle<int> bottomArea = area.removeFromBottom( area.getHeight() - previewArea.getHeight() );
	copier->setBounds(bottomArea.removeFromLeft(75).reduced(5));
	sequencer->setBounds(bottomArea.reduced(5));
}
