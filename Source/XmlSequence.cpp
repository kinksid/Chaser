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
	sequenceData = nullptr;
    positionData = nullptr;
	versionThreshold = version;
}

XmlSequence::~XmlSequence()
{
	
}

void XmlSequence::addElement(juce::XmlElement *elementToAddTo, juce::String nameOfNewElement, juce::String valueOfNewElement, bool replace)
{
	XmlElement* newElement = new XmlElement( nameOfNewElement );
	if ( replace && elementToAddTo->containsChildElement( newElement ))
		elementToAddTo->removeChildElement( newElement, false);
	
	newElement->addTextElement( valueOfNewElement );
	elementToAddTo->addChildElement( newElement );
}
void XmlSequence::setStep(int sequence, int step, Array<int> activeSlices)
{

    XmlElement* currentStep = sequenceData->getChildElement( sequence )->getChildElement( step );
    currentStep->deleteAllChildElements();
    
    //add child elements for each active slice in this step
    for ( int i = 0; i < activeSlices.size(); i++ )
    {
        
        //DBG("Added to XML: "+ String(i));
		addElement( currentStep, "slice", String( activeSlices[i]), false);
    }
}

Array<int> XmlSequence::getStep(int sequence, int step)
{
	Array<int> activeSlices;
	if (sequenceData != nullptr)
	{
		if (sequenceData->getChildElement(sequence) != nullptr)
		{
			if (sequenceData->getChildElement(sequence)->getChildElement(step) != nullptr)
			{
				XmlElement* currentStep = sequenceData->getChildElement(sequence)->getChildElement(step);

				//read out the the list of ints associated with this step
				for (int i = 0; i < currentStep->getNumChildElements(); i++)
				{
					activeSlices.add(currentStep->getChildElement(i)->getAllSubText().getIntValue());
				}
			}
		}
		
	}
    return activeSlices;
}

StringArray XmlSequence::getSequenceNames()
{
	StringArray names;
	if (sequenceData != nullptr)
	{
		if (sequenceData->getNumChildElements() > 0)
		{
			forEachXmlChildElement(*sequenceData, sequence)
			{
				names.add(sequence->getStringAttribute("name", "Unnamed Sequence"));
			}
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
	if (sequenceData != nullptr)
	{
		if (sequenceData->getNumChildElements() > 0)
		{
			forEachXmlChildElement(*sequenceData, sequence)
			{
				lengths.add(sequence->getIntAttribute("maxsteps", 16));
			}
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
	setPositionData(sliceXml, slice);
    positionData->addChildElement(sliceXml);

}

void XmlSequence::updateSlice(Slice *slice, int i )
{
	XmlElement* sliceXml = positionData->getChildElement(i);
	if ( sliceXml != nullptr )
	{
		setPositionData(sliceXml, slice);
	}
	
}

void XmlSequence::setPositionData(juce::XmlElement *sliceXml, Slice *slice)
{
	sliceXml->setAttribute("name", slice->name);
	sliceXml->setAttribute("enable", (int) slice->enabled );
	
	sliceXml->setAttribute("screenName", slice->screen);
	sliceXml->setAttribute("screenUniqueId", slice->uniqueId );
	
	sliceXml->deleteAllChildElements();

	XmlElement* inputRect = new XmlElement("inputRect");
	addPointsToXml(slice->inputRectPoints, inputRect);
	inputRect->setAttribute("orientation", slice->inputRectOrientation );
	sliceXml->addChildElement(inputRect);
	
	XmlElement* mask = new XmlElement("mask");
	addPointsToXml(slice->maskPoints, mask);
	sliceXml->addChildElement(mask);
	
	XmlElement* maskRect = new XmlElement("maskRect");
	addPointsToXml(slice->maskRectPoints, maskRect);
	maskRect->setAttribute("orientation", slice->maskRectOrientation);
	sliceXml->addChildElement(maskRect);
	
//	sliceXml->setAttribute("l", slice->proportionalX);
//	sliceXml->setAttribute("t", slice->proportionalY);
//	sliceXml->setAttribute("r", slice->proportionalX + slice->proportionalW);
//	sliceXml->setAttribute("b", slice->proportionalY + slice->proportionalH);
}

void XmlSequence::addPointsToXml(Array<Point<float> > &points, juce::XmlElement *pointDataElement)
{
	for ( int i = 0; i < points.size(); i++ )
	{
		XmlElement* point = new XmlElement("point");
		point->setAttribute("x", points[i].x);
		point->setAttribute("y", points[i].y);
		pointDataElement->addChildElement(point);
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
		if ( chaserData->getChildByName("version") != nullptr)
			return chaserData->getChildByName("version")->getAllSubText();
	
	return String();
}

void XmlSequence::setVersion(juce::String version)
{
	//delete the previous version
	if (chaserData->getChildByName("version") != nullptr )
		chaserData->removeChildElement(chaserData->getChildByName("version"), true);

	addElement( chaserData, "version", version, true);
}

void XmlSequence::createFreshXml( String version )
{
    
    //master element
    chaserData = new XmlElement ("chaserData");
	XmlElement* versionData = new XmlElement("version");
	versionData->addTextElement(version);
	chaserData->addChildElement( versionData );
	
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
	
	resolution = Point<int>(1920,1080);
	
	setXmlFile( getXmlFile() );
	
    clearSlices(); 
}

Array<Slice> XmlSequence::getSlices()
{
    Array<Slice> sliceArray;
    if ( positionData != nullptr )
    {
		forEachXmlChildElement(*positionData, slice)
		{
			Slice newSlice(slice->getStringAttribute("name", "Unnamed Slice"), slice->getIntAttribute("enable", 0) != 0 );
			if ( slice->hasAttribute("screenName"))
				newSlice.screen = slice->getStringAttribute("screenName");
			if ( slice->hasAttribute("screenUniqueId"))
				newSlice.uniqueId = slice->getIntAttribute("screenUniqueId");
			forEachXmlChildElement(*slice, pointData)
			{
				forEachXmlChildElement(*pointData, point)
				{
					float x = float(point->getDoubleAttribute("x"));
					float y = float(point->getDoubleAttribute("y"));
					Point<float> newPoint(x,y);
					if ( pointData->getTagName() == "inputRect" )
					{
						newSlice.inputRectOrientation = pointData->getStringAttribute("orientation", "0.0").getFloatValue();
						newSlice.inputRectPoints.add(newPoint);
					}
					if ( pointData->getTagName() == "mask" )
						newSlice.maskPoints.add(newPoint);
					if ( pointData->getTagName() == "maskRect" )
					{
						newSlice.maskRectOrientation = pointData->getStringAttribute("orientation", "0.0").getFloatValue();
						newSlice.maskRectPoints.add(newPoint);
					}
				}
			}
			sliceArray.add(newSlice);
		}
    }
    return sliceArray;
}

bool XmlSequence::save()
{
    //save everything into an XML file
    File f = getXmlFile();
	
	if ( f != File() )
	{
		if (! f.exists() )
			f.create();
		
		if (chaserData == nullptr)
			createFreshXml( getVersion() );

		if (!chaserData->writeToFile(f, "") )
			return false;
		
		//save the preferences
		setXmlFile( f );
		
		return true;
	}
	
	return false;
		
		/*
		else
		{
			DBG("SAVE ERROR!");
			AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
											  "Sorry!",
											  "Could not save data.",
											  "Ok");
		}
	}
	else
	{
		DBG("SAVE ERROR!");
		AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
										  "Sorry!",
										  "Could not save data.",
										  "Ok");
	}
		 */
	
	
}

void XmlSequence::setAssFile( File f )
{
	addElement( chaserData, "file", f.getFullPathName(), true );
    save();
}

File XmlSequence::getAssFile()
{
	if ( chaserData!=nullptr )
	{
		if ( chaserData->getChildByName("file") != nullptr )
		{
			return File(chaserData->getChildByName("file")->getAllSubText());
		}
	}
	
	return File();
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




bool XmlSequence::loadXmlFile( File f )
{
	//clear out the previous data
	if ( positionData != nullptr )
		positionData->deleteAllChildElements();
	
	//read everything out of the XML file, if it exists
	if (f.exists())
	{
		
		//read in the xml data
		XmlDocument dataDoc ( f );
		chaserData = dataDoc.getDocumentElement();
		if (versionCheck(getVersion(), versionThreshold))
		{
			if (chaserData->getChildByName("sequenceData") != nullptr)
			{
				sequenceData = chaserData->getChildByName("sequenceData");


				forEachXmlChildElement(*sequenceData, sequence)
				{
					forEachXmlChildElement(*sequence, step)
					{

					}
				}
			}
			
			if (chaserData->getChildByName("positionData") != nullptr)
			{
				positionData = chaserData->getChildByName("positionData");

				forEachXmlChildElement(*positionData, slice);
				{

				}
			}
			
			if ( chaserData->getChildByName("width") != nullptr )
				resolution.x = chaserData->getChildByName("width")->getAllSubText().getIntValue();
			else
				resolution.x = 1920;
			
			if ( chaserData->getChildByName("height") != nullptr )
				resolution.y = chaserData->getChildByName("height")->getAllSubText().getIntValue();
			else
				resolution.y = 1080;

			setXmlFile(f);
			return true;
		
	
		}
		
		else
			return false;
	}
	
	else //create everything from scratch
		return false;
}

void XmlSequence::setResolution(Point<int> resolution)
{
	addElement(chaserData, "width", String(resolution.x), true);
	addElement(chaserData, "height", String(resolution.y), true);
}

Point<int> XmlSequence::getResolution()
{
	return resolution;
}

void XmlSequence::setXmlFile( File f )
{
	if ( !f.exists() )
		f.create();
	
	File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
	File prefFile = docDir.getChildFile("Chaser/preferences/preferences.xml");
	if (!prefFile.exists())
		prefFile.create();
	
	XmlElement* lastUsedFileData = new XmlElement("preferences");
	addElement( lastUsedFileData, "lastusedfile", f.getFullPathName(), true );
	
	lastUsedFileData->writeToFile( prefFile, "");
	delete lastUsedFileData;
}

File XmlSequence::getXmlFile()
{
	File savedFileName = getXmlFileFromPreferences();
	if ( savedFileName != File() )
		return savedFileName;
	
	File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
	File defaultChaseFile = docDir.getChildFile("Chaser/DefaultChaser.xml");
	if (!defaultChaseFile.exists() )
		defaultChaseFile.create();

	return defaultChaseFile;
}

File XmlSequence::getXmlFileFromPreferences()
{
	//the preferences file we'll use the userDocs
	File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
	File prefFile = docDir.getChildFile("Chaser/preferences/preferences.xml");
	
	if ( prefFile.exists() )
	{
		XmlDocument lastUsedFile ( prefFile );
		lastUsedFileData = lastUsedFile.getDocumentElement();
		if (lastUsedFileData->getChildByName("lastusedfile") != nullptr )
		{
			File savedFile = File (lastUsedFileData->getChildByName("lastusedfile")->getAllSubText());
			if ( savedFile.exists() )
				return savedFile;
		}
		
	}
	
	return File();
}

