/*
  ==============================================================================

    ChaserXmlParser.cpp
    Created: 20 Feb 2016 11:51:34pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "ChaserXmlParser.h"

ChaserXmlParser::ChaserXmlParser()
{
	
}

ChaserXmlParser::~ChaserXmlParser()
{
	
}

void ChaserXmlParser::parseResolution(juce::File chaserFile, Point<int>& resolution )
{
	ScopedPointer<XmlElement> chaserData = getRoot( chaserFile );
	if ( chaserData )
	{
		if ( chaserData->getChildByName("width") != nullptr )
			resolution.x = chaserData->getChildByName("width")->getAllSubText().getIntValue();
		else
			resolution.x = 1920;
		
		if ( chaserData->getChildByName("height") != nullptr )
			resolution.y = chaserData->getChildByName("height")->getAllSubText().getIntValue();
		else
			resolution.y = 1080;
	}
}

void ChaserXmlParser::parseSequences(juce::File chaserFile, std::map<int, std::map<int, Array<int> > > &)
{
	ScopedPointer<XmlElement> chaserData = getRoot( chaserFile );
	if ( chaserData )
	{
		if (chaserData->getChildByName("sequenceData") != nullptr)
		{
			XmlElement* sequenceData = chaserData->getChildByName("sequenceData");
			
			
			forEachXmlChildElement(*sequenceData, sequence)
			{
				forEachXmlChildElement(*sequence, step)
				{
					
				}
			}
		}
		
		if (chaserData->getChildByName("positionData") != nullptr)
		{
			XmlElement* positionData = chaserData->getChildByName("positionData");
			
			forEachXmlChildElement(*positionData, slice);
			{
				
			}
		}
	}
}
		
bool ChaserXmlParser::canThisAppVersionLoadThisChaser(juce::String version, juce::File chaserFile)
{
	ScopedPointer<XmlElement> chaserData = getRoot( chaserFile );
	if ( chaserData != nullptr)
	{
		if ( chaserData->getChildByName("version") != nullptr)
		{
			String savedVersion = chaserData->getChildByName("version")->getAllSubText();
			return isVersionNewer( savedVersion, version );
		}
	}

	
	return false;
}

bool ChaserXmlParser::isVersionNewer(juce::String savedVersion, juce::String thisVersion)
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

XmlElement* ChaserXmlParser::getRoot( File chaserFile )
{
	if (chaserFile.exists())
	{
		//read in the xml data
		XmlDocument dataDoc ( chaserFile );
		return dataDoc.getDocumentElement();
	}
	
	return nullptr;
}

Array<int> ChaserXmlParser::subDivideString(juce::String s)
{
	Array<int> returnIntArray;
	StringArray vArray;
	vArray.addTokens ( s, ".", "\"");
	for (int i=0; i<vArray.size(); i++)
		returnIntArray.add(vArray[i].getIntValue());
	return returnIntArray;
}