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
    openOutput->setButtonText("Open ASS XML file");
    openOutput->addListener( this );
    addAndMakeVisible(openOutput);
    
    previewWindow = new Preview();
    addAndMakeVisible( previewWindow );
    setSize (1024, 768);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::buttonClicked(juce::Button *b)
{
    if ( b == openOutput );
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
}

void MainContentComponent::parseXml(juce::File f)
{
    XmlDocument xmlDoc ( f );
    ScopedPointer<XmlElement> preset (XmlDocument::parse ( f ));
    
    //traverse the whole fucking tree
    if (preset != nullptr && preset->hasTagName ("preset"))
    {
        //set the window name
        getParentComponent()->setName(preset->getStringAttribute("name"));
        
        //clear the previewWindow
        previewWindow->clearSlices();
        
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
                                
                                //type 0 means no crops
                                if ( type == 0 )
                                {
                                    SliceButton* sliceButton = new SliceButton( name, enabled, l, t, r, b);
                                    previewWindow->addSlice( sliceButton );
                                }
                            }
                        }
                    }
                }
            }
        }
        
        //update the previewWindow
        previewWindow->resized();
    }
}

void MainContentComponent::resized()
{
    
    float scale = 0.84;
    
    // adjust the heightt
    float wProp = getWidth() / 1920.0;
    float h = (wProp * 1080.0 ) / getHeight();
    
    previewWindow->setBoundsRelative(0.01 , 0.01, scale , h * scale);
    openOutput->setBoundsRelative(0.01, h * scale + 0.02, 0.15, 0.04);
}
