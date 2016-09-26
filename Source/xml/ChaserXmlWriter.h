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
sequencedata
slicedata
setupdata
to an xml file*/

class ChaserXmlWriter
{
public:
	ChaserXmlWriter();
	~ChaserXmlWriter();

	void saveDataToFile( File fileToSave, XmlElement* dataToSave );

private:

};



#endif  // CHASERXMLWRITER_H_INCLUDED
