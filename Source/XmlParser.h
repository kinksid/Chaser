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
	bool parseRes4Xml( XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices);
	//bool parseRes5Xml(File fileToParse, Array<Slice>& slices);
}



#endif  // XMLPARSER_H_INCLUDED
