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

void XmlSequence::addElement( juce::XmlElement *elementToAddTo, juce::String nameOfNewElement, juce::String valueOfNewElement, bool replace )
{
	XmlElement* newElement = new XmlElement( nameOfNewElement );
	if ( replace && elementToAddTo->containsChildElement( newElement ) )
		elementToAddTo->removeChildElement( newElement, false );

	newElement->addTextElement( valueOfNewElement );
	elementToAddTo->addChildElement( newElement );
}

void XmlSequence::setPositionData( juce::XmlElement *sliceXml, Slice *slice )
{
	sliceXml->setAttribute( "name", slice->sliceId.second );
	sliceXml->setAttribute( "sliceUniqueId", String( slice->sliceId.first ) );
	sliceXml->setAttribute( "enable", (int)slice->enabled );

	sliceXml->setAttribute( "screenName", slice->screenId.second );
	sliceXml->setAttribute( "screenUniqueId", String( slice->screenId.first ) );

	sliceXml->deleteAllChildElements();

	XmlElement* inputRect = new XmlElement( "inputRect" );
	addPointsToXml( slice->inputRectPoints, inputRect );
	inputRect->setAttribute( "orientation", slice->inputRectOrientation );
	sliceXml->addChildElement( inputRect );

	XmlElement* mask = new XmlElement( "mask" );
	addPointsToXml( slice->maskPoints, mask );
	sliceXml->addChildElement( mask );

	XmlElement* maskRect = new XmlElement( "maskRect" );
	addPointsToXml( slice->maskRectPoints, maskRect );
	maskRect->setAttribute( "orientation", slice->maskRectOrientation );
	sliceXml->addChildElement( maskRect );
}

void XmlSequence::addPointsToXml( Array<Point<float> > &points, juce::XmlElement *pointDataElement )
{
	for ( int i = 0; i < points.size(); i++ )
	{
		XmlElement* point = new XmlElement( "point" );
		point->setAttribute( "x", points[ i ].x );
		point->setAttribute( "y", points[ i ].y );
		pointDataElement->addChildElement( point );
	}
}

String XmlSequence::getVersion()
{
	return String();
}

void XmlSequence::setVersion( juce::String version )
{
	//delete the previous version
	if ( chaserData->getChildByName( "version" ) != nullptr )
		chaserData->removeChildElement( chaserData->getChildByName( "version" ), true );

	addElement( chaserData, "version", version, true );
}

void XmlSequence::createFreshXml( String version )
{
	//master element
	chaserData = new XmlElement( "chaserData" );
	XmlElement* versionData = new XmlElement( "version" );
	versionData->addTextElement( version );
	chaserData->addChildElement( versionData );

	//sequencing data
	//this is where we store which slices are active in each step of each sequence
	sequenceData = new XmlElement( "sequenceData" );
	chaserData->addChildElement( sequenceData );
}



bool XmlSequence::save()
{
	//save everything into an XML file
	File f = getXmlFile();

	if ( f != File() )
	{
		if ( !f.exists() )
			f.create();

		if ( chaserData == nullptr )
			createFreshXml( getVersion() );

		if ( !chaserData->writeToFile( f, "" ) )
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
	if ( chaserData != nullptr )
	{
		if ( chaserData->getChildByName( "file" ) != nullptr )
		{
			return File( chaserData->getChildByName( "file" )->getAllSubText() );
		}
	}

	return File();
}







bool XmlSequence::loadXmlFile( File f )
{
	//clear out the previous data
	if ( positionData != nullptr )
		positionData->deleteAllChildElements();

	//read everything out of the XML file, if it exists
	if ( f.exists() )
	{

		//read in the xml data
		XmlDocument dataDoc( f );
		chaserData = dataDoc.getDocumentElement();
		//		if (versionCheck(getVersion(), versionThreshold))
		{
			if ( chaserData->getChildByName( "sequenceData" ) != nullptr )
			{
				sequenceData = chaserData->getChildByName( "sequenceData" );


				forEachXmlChildElement( *sequenceData, sequence )
				{
					forEachXmlChildElement( *sequence, step )
					{

					}
				}
			}

			if ( chaserData->getChildByName( "positionData" ) != nullptr )
			{
				positionData = chaserData->getChildByName( "positionData" );

				forEachXmlChildElement( *positionData, slice );
				{

				}
			}
			setXmlFile( f );
			return true;


		}

		//else
		return false;
	}

	else //create everything from scratch
		return false;
}

void XmlSequence::setXmlFile( File f )
{
	if ( !f.exists() )
		f.create();

	File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
	File prefFile = docDir.getChildFile( "Chaser/preferences/preferences.xml" );
	if ( !prefFile.exists() )
		prefFile.create();

	XmlElement* lastUsedFileData = new XmlElement( "preferences" );
	addElement( lastUsedFileData, "lastusedfile", f.getFullPathName(), true );

	lastUsedFileData->writeToFile( prefFile, "" );
	delete lastUsedFileData;
}

File XmlSequence::getXmlFile()
{
	File savedFileName;// = getXmlFileFromPreferences();
	if ( savedFileName != File() )
		return savedFileName;

	File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
	File defaultChaseFile = docDir.getChildFile( "Chaser/DefaultChaser.xml" );
	if ( !defaultChaseFile.exists() )
		defaultChaseFile.create();

	return defaultChaseFile;
}



