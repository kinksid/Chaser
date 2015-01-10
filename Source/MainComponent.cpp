/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    laf = new LookAndFeel_V3;
    setLookAndFeel(laf);
    
    openOutput = new TextButton ("OpenOutput");
    openOutput->setButtonText("Load");
    openOutput->addListener( this );
    addAndMakeVisible(openOutput);
    
    update = new TextButton ("Update");
    update->setButtonText("Update");
    update->addListener( this );
    addAndMakeVisible(update);
    
    previewWindow = new Preview();
    addAndMakeVisible( previewWindow );
    
    sliceList = new SliceList();
    addAndMakeVisible(sliceList);
    sliceList->addChangeListener(this);
    
    sequencer = new Sequencer();
    addAndMakeVisible(sequencer);
    sequencer->addChangeListener(this);
    
    setSize (1024, 768);
    
    //while we're testing just always open the same test file
    activeFile = File ( "/Users/hybrid/Documents/Resolume Arena 4/presets/screensetup/MidTest.xml" );
    parseXml( activeFile );
    
    
}

MainContentComponent::~MainContentComponent()
{
    slices.clear();
    previewWindow = nullptr;
    sliceList = nullptr;
    
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
            activeFile = f;
            parseXml( activeFile );
        }
    }
    
    if ( b == update )
    {
        if ( activeFile.exists() )
            parseXml( activeFile );
    }
}


void MainContentComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if ( source == sliceList )
    {
        for ( int i = 0; i < slices.size(); i++ )
        {
            previewWindow->update(slices[i], i );
        }
    }
    
    if ( source == sequencer )
    {
        step = sequencer->activeButton;
    }
}

void MainContentComponent::parseXml(juce::File f)
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
            
            //update the previewWindow and sliceList
            sliceList->clearSlices();
            previewWindow->clearSlices();
            for ( int i = 0; i < slices.size(); i++ )
            {
                sliceList->addSlice( slices[i] );
                previewWindow->addSlice( slices[i] );
                
            }
            previewWindow->resized();
            
            
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
    openOutput->setBoundsRelative( scale + 0.02, h * scale - 0.03, (1.0 - scale - 0.03 ) / 2.0 - 0.005 , 0.04);
    update->setBoundsRelative( scale + (1.0 - scale  ) / 2.0 + 0.01, h * scale -0.03, (1.0 - scale - 0.03 ) / 2.0 -0.005 , 0.04);
    sequencer->setBoundsRelative( 0.01, h * scale + 0.02, scale, 0.1);
}
