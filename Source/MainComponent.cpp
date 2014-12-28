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
                                        forEachXmlChildElement( *slicesNode, SliceNode)
                                        {
                                            if ( SliceNode != nullptr && SliceNode->hasTagName("rect") )
                                            {
                                                float l = SliceNode->getDoubleAttribute( "l" );
                                                float t = SliceNode->getDoubleAttribute( "t" );
                                                float r = SliceNode->getDoubleAttribute( "r" );
                                                float b = SliceNode->getDoubleAttribute( "b" );
                                                Rectangle<float> rectangle = Rectangle<float> (l, t, r - l, b - t );
                                                rects.add( rectangle );
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
}

void MainContentComponent::resized()
{
    openOutput->setBoundsRelative(0.01, 0.01, 0.2, 0.05);
}
