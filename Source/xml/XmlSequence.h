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
    Array<Slice*> getSlices();
	
	StringArray getSequenceNames();
	void setSequenceName ( int sequenceNumber, String name );
	
	void setNumberOfSteps( int sequenceNumber, int numberOfSteps );
	Array<int> getSequenceLengths();

	String getVersion();
	void setVersion ( String version );
    
    void setAssFile ( File f );
    File getAssFile();
	
	void setResolution( Point<int> resolution );
	Point<int> getResolution();

    void createFreshXml( String version );
	
	
	
    bool save();
	bool loadXmlFile( File f );
    File getXmlFile();
	
	void setXmlFile( File f );
    
private:
	String versionThreshold;
    ScopedPointer<XmlElement> chaserData;
	ScopedPointer<XmlElement> lastUsedFileData;
    XmlElement* sequenceData;
    XmlElement* positionData;
	StringArray sequenceNames;
	Point<int> resolution;
	
	void addElement(XmlElement* elementToAddTo, String nameOfNewElement, String valueOfNewElement, bool replace);
	void setPositionData(XmlElement* sliceXml, Slice* s);
	void addPointsToXml(Array<Point<float>>& points, XmlElement* pointDataElement);
};



#endif  // XMLSEQUENCE_H_INCLUDED
