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
    
    openOutput = new TextButton ("OpenOutput");
    openOutput->setButtonText("Load");
	openOutput->setColour(TextButton::buttonColourId, laf->getOutlineColour());
    openOutput->addListener( this );
    addAndMakeVisible(openOutput);
        
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
    
    xmlSequence = new XmlSequence();
	
	//init the model vars
	currentSequence = 0;
	currentStep = 0;
    
    //load the slices from the previous xml
    if ( xmlSequence->getFile().exists() )
    {
		activeFile = xmlSequence->getFile();
        parseXml( xmlSequence->getFile() );
        //update the view for the first step
		activeSlices = xmlSequence->getStep( currentSequence, currentStep );
        previewWindow->setSlices( activeSlices );
		sequencer->setSequenceNames ( xmlSequence->getSequenceNames() );
    }
    else
    {
        xmlSequence->createFreshXml();
    }
	
    setSize (1024, 600);
}

MainContentComponent::~MainContentComponent()
{
    slices.clear();
    previewWindow = nullptr;
    sliceList = nullptr;
    xmlSequence = nullptr;
    
}

void MainContentComponent::buttonClicked(juce::Button *b)
{
    if ( b == openOutput )
    {
        File presetsLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 4/presets/screensetup/";
        FileChooser fc ( "Pick an ASS file...", presetsLocation, "*.xml", true );
        if ( fc.browseForFileToOpen() );
        {
            File f = fc.getResult();
            parseXml( f );
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
    }
}

void MainContentComponent::copierClicked(int multiplier)
{
	
	for ( int i = multiplier; i < 16; i += multiplier )
	{
		int nextStep = currentStep + i;
		if ( nextStep >= 16 )
			nextStep -= 16;
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
			xmlSequence->createFreshXml();
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
					{
						slices[i]->enabled = prevSlices[i].enabled;
						
					}
					

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
			
			//set the previewWindow to the correct step
			
			//if we're not loading the same file, reset the sequencer
			if ( !loadingSameFile )
			{
				sequencer->setDefaultSequences();
			}
			
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
    
    float scale = 0.84;
    
    // adjust the heightt
    float wProp = getWidth() / 1920.0;
    float h = (wProp * 1080.0 ) / getHeight();
    Rectangle <int> rect = getBounds();
    
    previewWindow->setBoundsRelative( 0.01 , 0.01, scale , h * scale );
    sliceList->setBoundsRelative( scale + 0.02, 0.01, 1.0 - scale - 0.03, h * scale - 0.05);
    openOutput->setBoundsRelative( scale + 0.02, h * scale - 0.035, (1.0 - scale - 0.03 ) - 0.005 , 0.04);
    sequencer->setBoundsRelative( 0.06, h * scale + 0.02, 0.93, 1.0 - h * scale - 0.04);
	copier->setBoundsRelative(0.01, h * scale + 0.02, 0.045, 1.0 - h * scale - 0.04);
}
