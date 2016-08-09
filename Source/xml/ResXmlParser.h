/*
  ==============================================================================

    ResXmlParser.h
    Created: 9 Sep 2015 8:45:41pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef RESXMLPARSER_H_INCLUDED
#define RESXMLPARSER_H_INCLUDED

#include "JuceHeader.h"
#include "../slice/Slice.h"

//helper class to parse xml data in the various resolume files

class ResXmlParser
{
public:
	ResXmlParser();
	~ResXmlParser();
	
	static bool parseAssXml ( File f, OwnedArray<Slice>& slices, Point<int>& resolution );
	
	//TODO make this take a File, not a XmlElement
	static String getAdvancedPresetNameFromRes5Xml( XmlElement& xmlTreeToParse );
	
	
	
private:
	static bool parseRes4Xml( XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices, Point<int>& resolution );
	static bool parseRes5Xml( XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices, Point<int>& resolution );
	static bool parseRes5PrefXml ( XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices, Point<int>& resolution );
	static bool parseRes4ConfigXml ( XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices, Point<int>& resolution );
	
	
	
	static void addPointToSlice ( XmlElement* sliceElement, Array<Point<float>>& pointType, Point<int> resolution);
	
	
};



#endif  // RESXMLPARSER_H_INCLUDED
