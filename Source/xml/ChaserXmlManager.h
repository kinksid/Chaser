/*
  ==============================================================================

    ChaserXmlManager.h
    Created: 26 Sep 2016 6:38:51pm
    Author:  Joris

  ==============================================================================
*/

#ifndef CHASERXMLMANAGER_H_INCLUDED
#define CHASERXMLMANAGER_H_INCLUDED

#include "JuceHeader.h"

/*ChaserXmlWriter takes care of writing
xml data
to an xml file*/

class ChaserXmlManager
{
public:
	ChaserXmlManager();
	~ChaserXmlManager();

	void saveXmlElement( XmlElement* dataToSave );

	void setSaveFile( File newSaveFile );

private:
	bool save( XmlElement* toSave );
	XmlElement* getMainElement();

	//todo by setting a mainelement name, i can probably make this an api thing

	File saveFile;
};



#endif  // CHASERXMLWRITER_H_INCLUDED
