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
            XmlDocument xmlDoc ( f );
            ScopedPointer<XmlElement> preset (XmlDocument::parse ( f ));
            
            //traverse the whole fucking tree
            if (preset != nullptr && preset->hasTagName ("preset"))
            {
                //set the window name
                getParentComponent()->setName(preset->getStringAttribute("name"));
                
                //clear previous slice
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
                                        if ( name != "" && type == 0 )
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
            }
            
            
        }
    }
}

void MainContentComponent::resized()
{
    float h = ( 0.98  * getHeight() ) / 1080.0 ;
    previewWindow->setBoundsRelative(0.01 , 0.01, 0.98 , h );
    openOutput->setBoundsRelative(0.01, h + 0.02, 0.2, 0.05);
}
