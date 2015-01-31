/*
  ==============================================================================

    XmlSequence.cpp
    Created: 11 Jan 2015 12:04:53am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "XmlSequence.h"


XmlSequence::XmlSequence()
{
    positionData = nullptr;
    load();
}

XmlSequence::~XmlSequence()
{

}

void XmlSequence::setStep(int sequence, int step, Array<int> activeSlices)
{

    XmlElement* currentStep = sequenceData->getChildElement( sequence )->getChildElement( step );
    currentStep->deleteAllChildElements();
    
    //add child elements for each active slice in this step
    for ( int i = 0; i < activeSlices.size(); i++ )
    {
        
        //DBG("Added to XML: "+ String(i));
        XmlElement* slice = new XmlElement( "slice" );
        slice->setAttribute("nr", activeSlices[i]);
        currentStep->addChildElement(slice);
    }
}

Array<int> XmlSequence::getStep(int sequence, int step)
{
    
    XmlElement* currentStep = sequenceData->getChildElement( sequence )->getChildElement( step );
    Array<int> activeSlices;
    
    //read out the the list of ints associated with this step
    for ( int i = 0; i < currentStep->getNumChildElements(); i++ )
    {
        //DBG("Read from XML: " + String(currentStep->getChildElement(i)->getTagName().getIntValue()));
        activeSlices.add( currentStep->getChildElement(i)->getIntAttribute("nr") );
    }
    return activeSlices;
}

StringArray XmlSequence::getSequenceNames()
{
	StringArray names;
	if ( sequenceData->getNumChildElements() > 0 )
	{
		forEachXmlChildElement(*sequenceData, sequence)
		{
			names.add(sequence->getStringAttribute("name"));
		}
	}
	
	return names;
}

void XmlSequence::setSequenceNames(juce::StringArray names)
{
	if ( sequenceData->getNumChildElements() > 0 )
	{
		int i = 0;
		forEachXmlChildElement(*sequenceData, sequence)
		{
			sequence->setAttribute("name", names[i]);
			i++;
		}
	}

}

void XmlSequence::addSlice(Slice* slice)
{
    
    if ( positionData == nullptr )
    {
        positionData = new XmlElement("positionData");
        chaserData->addChildElement(positionData);
    }

    XmlElement* sliceXml = new XmlElement("slice");
    sliceXml->setAttribute("name", slice->name);
    sliceXml->setAttribute("enable", (int) slice->enabled );
    sliceXml->setAttribute("l", slice->proportionalX);
    sliceXml->setAttribute("t", slice->proportionalY);
    sliceXml->setAttribute("r", slice->proportionalX + slice->proportionalW);
    sliceXml->setAttribute("b", slice->proportionalY + slice->proportionalH);
    positionData->addChildElement(sliceXml);

}

void XmlSequence::clearSlices()
{
    if ( positionData != nullptr )
    {
        positionData->deleteAllChildElements();
    }
}

void XmlSequence::createFreshXml()
{
    
    //master element
    chaserData = new XmlElement ("chaserData");
    
    
    //sequencing data
    //this is where we store which slices are active in each step of each sequence
    sequenceData = new XmlElement ("sequenceData");
    chaserData->addChildElement(sequenceData);
    
    //create 16 sequences
    for ( int i = 0; i < 16; i++ )
    {
        XmlElement* sequence = new XmlElement ("sequence");
        sequence->setAttribute("nr", i);
		sequence->setAttribute("name", "Sequence " + String(i + 1));
        sequenceData->addChildElement(sequence);
        
        //each with 16 steps
        for ( int j = 0; j < 16; j++ )
        {
            XmlElement* step = new XmlElement ("step");
            step->setAttribute("nr", j);
            sequence->addChildElement(step);
        }
    }
    
    clearSlices();

    
    
}

Array<Slice> XmlSequence::getSlices()
{
    Array<Slice> sliceArray;
    if ( positionData != nullptr )
    {
        if ( positionData->getNumChildElements() > 0 )
        {
            forEachXmlChildElement(*positionData, slice)
            sliceArray.add(Slice(slice->getStringAttribute("name"), (bool)slice->getIntAttribute("enable"), slice->getDoubleAttribute("l"), slice->getDoubleAttribute("t"), slice->getDoubleAttribute("r"), slice->getDoubleAttribute("b")));
        }
    }
    return sliceArray;
}

void XmlSequence::save()
{
    //save everything into an XML file
    File f = getXmlFile();
    if (! f.exists() )
        f.create();
    
    if (chaserData->writeToFile(f, "") )
        return;
    else
        DBG("SAVE ERROR!");
}

void XmlSequence::setFile( File f )
{
    String fileName = f.getFullPathName();
    chaserData->setAttribute( "file", fileName );
    save();
}

File XmlSequence::getFile()
{

    return File(chaserData->getStringAttribute("file"));
}

void XmlSequence::load()
{
    
    //read everything out of the XML file, if it exists
    File f = getXmlFile();
    if (f.exists())
    {
        //read in the xml data
        XmlDocument dataDoc ( f );
        chaserData = dataDoc.getDocumentElement();
        sequenceData = chaserData->getChildByName("sequenceData");
       
        forEachXmlChildElement(*sequenceData, sequence)
        {
            forEachXmlChildElement(*sequence, step)
            {
                
            }
        }
        
        positionData = chaserData->getChildByName( "positionData");
        if ( positionData != nullptr)
        {
            forEachXmlChildElement(*positionData, slice);
        }
    }
    
    else //create everything from scratch
    {
        createFreshXml();
    }
}

File XmlSequence::getXmlFile()
{
    //get the file chaserData.xml, for now we'll use the userDocs
    File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
    File xmlFile = docDir.getChildFile("Chaser/chaserdata.xml");
    return xmlFile;
}

