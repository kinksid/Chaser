/*
  ==============================================================================

    FileHelper.cpp
    Created: 20 Feb 2016 5:58:44pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "FileHelper.h"
#include "ResXmlParser.h"


FileHelper::FileHelper()
{
	
}

FileHelper::~FileHelper()
{
	
}

File FileHelper::getAssFileFromUser()
{
	//first check Arena 5's location, if it doesn't exist, then check Arena 4's location
	File presetsLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 5/presets/screensetup/";
	if ( !presetsLocation.exists() )
		presetsLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 4/presets/screensetup/";
	
	//ask the user to pick a file
	FileChooser fc ( "Pick an Arena setup file...", presetsLocation, "*.xml", true );
	if ( fc.browseForFileToOpen() )
	{
		return fc.getResult();
		
//		//try to parse it
//		if (!parseAssXml( f ) )
//			throwLoadError();
	}
	else
		return File();
}

File FileHelper::getAssFileAutomagically()
{
	//check for the Arena 5 preset
	File advancedFile = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 5/preferences/screensetup/advanced.xml";
	
	if ( advancedFile.exists() )
	{
		if ( AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::QuestionIcon,
										  "Res 5 setup file found!",
										  "Would you like to load the current Arena setup file?",
										  "OK", "Cancel") )
		{
			//parse the advanced.xml file and see if it contains the file name of the xml that is currently loaded
			ScopedPointer<XmlElement> xmlRoot (XmlDocument::parse ( advancedFile ));
			String advancedName = ResXmlParser::getAdvancedPresetNameFromRes5Xml( *xmlRoot );
			
			//if we get a name, make a File out of it and return it
			if ( advancedName != String().empty )
			{
				File namedPreset = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 5/presets/screensetup/" + advancedName + ".xml";
				return namedPreset;
			}
			else
				//return the advanced file itself
				//when the user does not save a named preset
				//this file can also contains ass data
				return advancedFile;
		}
	}
	//check for the Arena 4 preset
	else
	{
		advancedFile = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 4/preferences/config.xml";
		if ( advancedFile.exists() )
		{
			
			if ( AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::QuestionIcon,
											  "Res 4 setup file found!",
											  "Would you like to load the current Arena setup file?",
											  "OK", "Cancel") )
			{
				//in Arena 4, the entire current ass preset is stored inside the config xml
				return advancedFile;
			}
		}
		
	}
	//if everything has failed, we return an empty file
	return File();
}

void FileHelper::reloadAssFile()
{
	//TODO add this back
//	if (!parseAssXml( xmlSequence->getAssFile() ))
//		throwLoadError();
}

File FileHelper::getPreferencesFile()
{
	//the preferences file is stored in the userDocs
	File docDir = File::getSpecialLocation( File::userDocumentsDirectory );
	File prefFile = docDir.getChildFile("Chaser/preferences/preferences.xml");
	
	return prefFile;
	/*
	if ( prefFile.exists() )
	{
		XmlDocument lastUsedFile ( prefFile );
		XmlElement* lastUsedFileData = lastUsedFile.getDocumentElement();
		if (lastUsedFileData->getChildByName("lastusedfile") != nullptr )
		{
			File savedFile = File (lastUsedFileData->getChildByName("lastusedfile")->getAllSubText());
			if ( savedFile.exists() )
				return savedFile;
		}
		
	}
	
	
	return File();
	  */
}

File FileHelper::getLastUsedChaserFile( File prefFile)
{
	 if ( prefFile.exists() )
	 {
		XmlDocument lastUsedFile ( prefFile );
		XmlElement* lastUsedFileData = lastUsedFile.getDocumentElement();
		if (lastUsedFileData->getChildByName("lastusedfile") != nullptr )
		{
			//TODO don't use subtext, but juce formatting
			File savedFile = File (lastUsedFileData->getChildByName("lastusedfile")->getAllSubText());
			if ( savedFile.exists() )
				return savedFile;
		}
		
	 }
	 return File();
}


void FileHelper::throwLoadError()
{
	AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
									 "Sorry!",
									 "Something went wrong reading that file.",
									 "Ok");
	DBG("Error loading file...");
}

void FileHelper::throwSaveError()
{
	AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
									  "Sorry!",
									  "Could not save data.",
									  "Ok");
}