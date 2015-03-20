/*
  ==============================================================================

    XmlSequence.cpp
    Created: 11 Jan 2015 12:04:53am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "XmlSequence.h"


XmlSequence::XmlSequence( String version )
{
    positionData = nullptr;
	versionThreshold = version;
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
        slice->addTextElement  (String( activeSlices[i]));
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
        activeSlices.add( currentStep->getChildElement(i)->getAllSubText().getIntValue() );
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
			names.add(sequence->getStringAttribute("name", "Unnamed Sequence"));
		}
	}
	
	return names;
}

void XmlSequence::setSequenceName( int sequenceNumber, String name)
{
	if( sequenceData->getNumChildElements() > sequenceNumber )
	{
		XmlElement* sequence = sequenceData->getChildElement( sequenceNumber );
		sequence->setAttribute("name", name );
	}
}

void XmlSequence::setNumberOfSteps( int sequenceNumber, int numberOfSteps )
{
	if( sequenceData->getNumChildElements() > sequenceNumber )
	{
		XmlElement* sequence = sequenceData->getChildElement( sequenceNumber );
		sequence->setAttribute( "maxsteps", numberOfSteps );
	}
}

Array<int> XmlSequence::getSequenceLengths()
{
	Array<int> lengths;
	if ( sequenceData->getNumChildElements() > 0 )
	{
		forEachXmlChildElement(*sequenceData, sequence)
		{
			lengths.add(sequence->getIntAttribute("maxsteps", 16));
		}
	}
	
	return lengths;
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

void XmlSequence::updateSlice(Slice *slice, int i )
{
	XmlElement* sliceXml = positionData->getChildElement(i);
	if ( sliceXml != nullptr )
	{
		sliceXml->setAttribute("name", slice->name);
		sliceXml->setAttribute("enable", (int) slice->enabled );
		sliceXml->setAttribute("l", slice->proportionalX);
		sliceXml->setAttribute("t", slice->proportionalY);
		sliceXml->setAttribute("r", slice->proportionalX + slice->proportionalW);
		sliceXml->setAttribute("b", slice->proportionalY + slice->proportionalH);
	}
	
}

void XmlSequence::clearSlices()
{
	chaserData->deleteAllChildElementsWithTagName("positionData");
	positionData = new XmlElement ("positionData");
	chaserData->addChildElement(positionData);
}

String XmlSequence::getVersion()
{
	if ( chaserData != nullptr)
		return chaserData->getStringAttribute("version");
	else
		return String();
}

void XmlSequence::setVersion(juce::String version)
{
	chaserData->setAttribute("version", version );
}

void XmlSequence::createFreshXml( String version )
{
    
    //master element
    chaserData = new XmlElement ("chaserData");
	chaserData->setAttribute("version", version );
	
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
		sequence->setAttribute( "maxsteps", 16 );
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
            sliceArray.add(Slice(slice->getStringAttribute("name", "Unnamed Slice"), (bool)slice->getIntAttribute("enable", false), slice->getDoubleAttribute("l", 0.0), slice->getDoubleAttribute("t", 0.0), slice->getDoubleAttribute("r", 0.0), slice->getDoubleAttribute("b", 0.0)));
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

Array<int> XmlSequence::subDivideString(juce::String s)
{
	Array<int> returnIntArray;
	StringArray vArray;
	vArray.addTokens ( s, ".", "\"");
	for (int i=0; i<vArray.size(); i++)
	{
		returnIntArray.add(vArray[i].getIntValue());
	}
	return returnIntArray;
	
}

bool XmlSequence::versionCheck(juce::String savedVersion, juce::String thisVersion)
{
	//if the savedversion string is empty, it's always out of date
	if ( savedVersion.isEmpty() )
		return false;
	
	//versions always have three components so break them up to individual parts
	Array<int> savedVersionInts = subDivideString ( savedVersion );
	Array<int> thisVersionInts = subDivideString ( thisVersion );
	
	//check each level
	//if they all match or are newer, we're good to go
	if ( savedVersionInts[0] >= thisVersionInts[0] )
		if ( savedVersionInts[1] >= thisVersionInts[1] )
			if ( savedVersionInts[2] >= thisVersionInts[2] )
				return true;
	
	return false;
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
		
		if ( versionCheck( getVersion(), versionThreshold ) )
		{
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
				{
					
				}
			}
		}
		
		else
			createFreshXml( getVersion() );
	}
    
    else //create everything from scratch
    {
        createFreshXml( getVersion() );
    }
}

File XmlSequence::getXmlFile()
{
    //get the file chaserData.xml, for now we'll use the userDocs
    File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
    File xmlFile = docDir.getChildFile("Chaser/chaserdata.xml");
    return xmlFile;
}

