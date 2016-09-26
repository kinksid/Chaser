/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "../../HybridApi/Source/HybridApi.h"



//==============================================================================
MainContentComponent::MainContentComponent() : chaseManager( new ChaseManager()), sliceManager ( new SliceManager())
{
	laf = new ColourLookAndFeel();
	
    setLookAndFeel(laf);
	
	version = ProjectInfo::versionString;
	
    previewWindow = new Preview();
    addAndMakeVisible( previewWindow );
    
    sliceList = new SliceList();
    addAndMakeVisible(sliceList);
    
    sequencer = new Sequencer(*this);
    addAndMakeVisible(sequencer);
	
	copier = new Copier();
	addAndMakeVisible(copier);
	
	//create a sequence and set the oldest version it will correctly load
	//	xmlSequence = new XmlSequence( "0.0.1" );

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
	
//	chaseManager = new ChaseManager();
//	sliceManager = new SliceManager();
	
	setSize (1280, 720);
	
	//start a timer to update the window name
	startTimer( 1000 );
	
	slicesToCopy.clear();
	currentSequenceSlices.clear();
	sequenceNameToCopy = String().empty;
}

MainContentComponent::~MainContentComponent()
{
	// slices.clear();
    previewWindow = nullptr;
    sliceList = nullptr;
	//    xmlSequence = nullptr;
	#if JUCE_MAC
	setMacMainMenu(nullptr);
	#endif
	slicesToCopy.clear();
	currentSequenceSlices.clear();
	
	chaseManager = nullptr;
	sliceManager = nullptr;
}

void MainContentComponent::timerCallback()
{
	stopTimer();
	/*
	//check if we have a Chaser file previously saved
	//if so, load that bad boy
	//File savedFile = xmlSequence->getXmlFileFromPreferences();
	File prefFile = fileHelper->getPreferencesFile();
	File lastUsedChaser = fileHelper->getLastUsedChaserFile( prefFile );
	
	if (lastUsedChaser != File() && lastUsedChaser.exists() )
	{
		ChaserXmlParser::parseResolution( lastUsedChaser, resolution );
		//this will return 1920x1080 if no resolution was saved
		resolution = xmlSequence->getResolution();
		reloadSliceData();
		//if something goes wrong here, fuck it
	}
	
	
	else
	  */
	{
//		xmlSequence->createFreshXml( version );
//		resolution = xmlSequence->getResolution();
//		saveXml();
		//see if there are any ass files
		//if so, load their slices into the slice mananger
		File assFile = FileHelper::getAssFileAutomagically( true );
		
		ResXmlParser::parseAssXml( assFile, sliceManager->getSlices(), sliceManager->getResolution());
		
		//now populate the previewwindow with buttons for these slices
		previewWindow->createSliceButtons( sliceManager->getSlices() );
		previewWindow->resized();
		
		//now populate the slicelist with entries for these slices
		sliceList->addSlices( sliceManager->getSlices() );
		sliceList->resized();
		//loadDefaultAssFile();
	}
	//set the name
	//	getTopLevelComponent()->setName( xmlSequence->getXmlFile().getFileNameWithoutExtension());
	
	//resize to update preview window
	resized();
	
	
}

StringArray MainContentComponent::getMenuBarNames()
{
	const char* const names[] = { "File", "Edit", nullptr };
	return StringArray (names);
}

PopupMenu MainContentComponent::getMenuForIndex(int menuIndex, const juce::String &)
{
	
	PopupMenu menu;
	
	if (menuIndex == 0)
	{
		menu.addItem(1, "Load Arena Setup");
		menu.addItem(2, "Reload Arena Setup", false /*xmlSequence->getAssFile() == File()*/? false: true );
		menu.addSeparator();
		menu.addItem(3, "New Chaser");
		menu.addSeparator();
		menu.addItem(4, "Load Chaser");
		menu.addItem(5, "Save Chaser as...");
	}
	
	else if ( menuIndex == 1 )
	{
		menu.addItem(1, "Copy Step " + KeyPress('c', ModifierKeys::commandModifier, NULL).getTextDescriptionWithIcons());
		menu.addItem(2, "Paste Step " + KeyPress('v', ModifierKeys::commandModifier, NULL).getTextDescriptionWithIcons());
		menu.addSeparator();
		menu.addItem(3, "Copy Sequence");
		menu.addItem(4, "Paste Sequence");
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
				/*
				xmlSequence->createFreshXml(version);
				resolution = xmlSequence->getResolution();
				loadAssFile();
				saveXml();
				 */
				break;
			case 2:
				/*
				reloadAssFile();
				 */
				break;
			case 3:
			{
				/*
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
				 */
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
	
	else if ( topLevelMenuIndex == 1 )
	{
		switch ( menuItemID )
		{
			case 1:
				copyStep();
				break;
			case 2:
				pasteStep();
				break;
			case 3:
				copySequence();
				break;
			case 4:
				pasteSequence();
				break;

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
	
	sliceList->clear();
	previewWindow->clearSlices();
	//xmlSequence->clearSlices();
	//sequencer->setDefaultSequences();
	resized();
}




void MainContentComponent::saveXml()
{
	/*
	if (!xmlSequence->save())
	{
		DBG("SAVE ERROR!");
		throwSaveError();
	}
	 */
}

bool MainContentComponent::saveAsXml()
{
	/*
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
	 */
	return false;
}

void MainContentComponent::loadXml()
{
	/*
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
	 */
}

void MainContentComponent::reloadSliceData()
{
	/*
	//sliceList->clearSlices();
	previewWindow->clearSlices();
	slices.clear();
	
	//load the slices from the xml if they exist
	Array<Slice*> slicesInXml = xmlSequence->getSlices();
	for ( int i = 0; i < slicesInXml.size(); i++ )
	{
		Slice* s = slicesInXml[i];
		previewWindow->addSlice (s) ;
		
		slices.add(s);
	}
	
	sliceList->addSlices( slices );
	
	//update the view for the first step
	activeSlices = xmlSequence->getStep( currentSequence, currentStep );
	previewWindow->setSlices( activeSlices );
	sequencer->setSequenceNames ( xmlSequence->getSequenceNames() );
	sequencer->setSequenceLengths ( xmlSequence->getSequenceLengths() );
	currentSequenceLength = xmlSequence->getSequenceLengths()[ currentSequence ];
	
	resolution = xmlSequence->getResolution();
	 */
	resized();
}

bool MainContentComponent::keyPressed(const juce::KeyPress &key, juce::Component *)
{
	
	if ( key == KeyPress::leftKey )
	{
		sequencer->previousStep();
	}
	else if ( key == KeyPress::rightKey )
	{
		sequencer->nextStep();
	}
	else if ( key == KeyPress ('c', ModifierKeys::commandModifier, NULL) )
	{
		copyStep();
	}
	else if ( key ==  KeyPress ('v', ModifierKeys::commandModifier, NULL))
	{
		pasteStep();
	}
		
	return true;
}

void MainContentComponent::copyStep()
{
	slicesToCopy = activeSlices;
}

void MainContentComponent::pasteStep()
{
	previewWindow->setActiveSlices( slicesToCopy );
}

void MainContentComponent::copySequence()
{
	/*
	//copy the name
	sequenceNameToCopy = xmlSequence->getSequenceNames()[currentSequence];
	//create an array with an array
	currentSequenceSlices.clear();
	for ( int i = 0; i < currentSequenceLength; i++ )
	{
		Array<int> step = xmlSequence->getStep(currentSequence, i);
		currentSequenceSlices.add( step );
	}
	 */
}

void MainContentComponent::pasteSequence()
{
	/*
	sequenceNameChanged( String(sequenceNameToCopy + " copy"));
	sequenceLengthChanged( currentSequenceSlices.size() );
	for ( int i = 0; i < currentSequenceSlices.size(); i++ )
	{
		stepSelected( i );
		sliceClicked( currentSequenceSlices[i] );
		previewWindow->setSlices( activeSlices );
	}
	
	//update the views
	sequencer->selectStep( 0 );
	sequencer->setSequenceNames ( xmlSequence->getSequenceNames() );
	sequencer->setSequenceLengths ( xmlSequence->getSequenceLengths() );
	 */
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
	
	previewWindow->setBounds(0,0, sliceManager->getResolution().x, sliceManager->getResolution().y);
	Rectangle<int> previewWindowArea = previewArea.reduced(5);

	if(previewWindow->getWidth() > 0 && previewWindow->getHeight() > 0 )
		previewWindow->setBoundsToFit( previewWindowArea.getX(), previewWindowArea.getY(), previewWindowArea.getWidth(), previewWindowArea.getHeight(), Justification::centred, false);
	
	Rectangle<int> sliceArea = Rectangle<int> { previewArea.getWidth(), menuBarHeight, area.getWidth() - previewArea.getWidth(), previewArea.getHeight()};
	sliceList->setBounds(sliceArea.reduced(5));

	Rectangle<int> bottomArea = area.removeFromBottom( area.getHeight() - previewArea.getHeight() );
	copier->setBounds(bottomArea.removeFromLeft(75).reduced(5));
	sequencer->setBounds(bottomArea.reduced(5));
}
