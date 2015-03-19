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
    XmlSequence();
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

    
    void setFile ( File f );
    File getFile();

    void createFreshXml();
    
    
    void save();
    void load();
    File getXmlFile();
    
private:
    ScopedPointer<XmlElement> chaserData;
    XmlElement* sequenceData;
    XmlElement* positionData;
	StringArray sequenceNames;
//    OwnedArray<XmlElement> slices;
//    OwnedArray<XmlElement> sequences;
//    OwnedArray<OwnedArray<XmlElement>> steps;
};



#endif  // XMLSEQUENCE_H_INCLUDED
