/*
  ==============================================================================

    ChaserXmlParser.h
    Created: 20 Feb 2016 11:51:34pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef CHASERXMLPARSER_H_INCLUDED
#define CHASERXMLPARSER_H_INCLUDED

#include "JuceHeader.h"
#include "Slice.h"
#include <map>

//helper class to parse the data in a chaser file

class ChaserXmlParser
{
public:
	ChaserXmlParser();
	~ChaserXmlParser();
	
	static void parseSlices ( File chaserFile, OwnedArray<Slice>& slices );
	static void parseResolution ( File chaserFile, Point<int>& resolution);
	static void parseSequences ( File chaserFile, std::map<int,std::map<int, Array<int>>>& );
	
	static bool canThisAppVersionLoadThisChaser ( String version, File chaserFile );
private:
	static XmlElement* getRoot( File chaserFile );
	static bool isVersionNewer ( String savedVersion, String thisVersion );
	static Array<int> subDivideString ( String s);
};



#endif  // CHASERXMLPARSER_H_INCLUDED
