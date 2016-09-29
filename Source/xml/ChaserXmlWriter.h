/*
  ==============================================================================

    ChaserXmlWriter.h
    Created: 26 Sep 2016 6:38:51pm
    Author:  Joris

  ==============================================================================
*/

#ifndef CHASERXMLWRITER_H_INCLUDED
#define CHASERXMLWRITER_H_INCLUDED

#include "JuceHeader.h"

/*ChaserXmlWriter takes care of writing
xml data
to an xml file*/

class ChaserXmlWriter
{
public:
	ChaserXmlWriter();
	~ChaserXmlWriter();

	void saveXmlElement( XmlElement* dataToSave );

	void setSaveFile( File newSaveFile );

private:
	bool save( XmlElement* toSave );
	XmlElement* getMainElement();

	//todo by setting a mainelement name, i can probably make this an api thing

	File saveFile;
};



#endif  // CHASERXMLWRITER_H_INCLUDED
