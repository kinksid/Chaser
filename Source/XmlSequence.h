/*
  ==============================================================================

    XmlSequence.h
    Created: 11 Jan 2015 12:04:53am
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef XMLSEQUENCE_H_INCLUDED
#define XMLSEQUENCE_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Slice.h"

//TODO add node with positions for all slices

class XmlSequence
{
public:
    XmlSequence( String version );
    ~XmlSequence();
    
    void setStep ( int sequence, int step, Array<int> activeSlices );
    Array<int> getStep ( int sequence, int step );

	
    void clearSlices();
    void addSlice ( Slice* slice );
	void updateSlice ( Slice* slice, int i);
    Array<Slice> getSlices();
	
	StringArray getSequenceNames();
	void setSequenceName ( int sequenceNumber, String name );
	
	void setNumberOfSteps( int sequenceNumber, int numberOfSteps );
	Array<int> getSequenceLengths();

	String getVersion();
	void setVersion ( String version );
    
    void setFile ( File f );
    File getFile();

    void createFreshXml( String version );
	bool versionCheck ( String savedVersion, String thisVersion );
	Array<int> subDivideString ( String s);
	
    void save();
    void load();
    File getXmlFile();
    
private:
	String versionThreshold;
    ScopedPointer<XmlElement> chaserData;
    XmlElement* sequenceData;
    XmlElement* positionData;
	StringArray sequenceNames;

};



#endif  // XMLSEQUENCE_H_INCLUDED
