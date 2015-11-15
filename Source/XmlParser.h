/*
  ==============================================================================

    XmlParser.h
    Created: 9 Sep 2015 8:45:41pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef XMLPARSER_H_INCLUDED
#define XMLPARSER_H_INCLUDED

#include "JuceHeader.h"
#include "Slice.h"

namespace XmlParser
{
	bool parseRes4Xml( XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices );
	bool parseRes5Xml( XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices, Point<int>& resolution );
	
	bool parseRes5PrefXml ( XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices, Point<int>& resolution );
	
	String getAdvancedPresetNameFromRes5Xml( XmlElement& xmlTreeToParse );
	
	void addPointToSlice ( Point<float> newPoint, XmlElement* sliceElement, Array<Point<float>>& pointType, Point<int> resolution);
}



#endif  // XMLPARSER_H_INCLUDED
