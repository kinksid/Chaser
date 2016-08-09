//
//  SvgFile.cpp
//  Chaser
//
//  Created by Joris on 12/01/16.
//
//

#include <stdio.h>
#include "SvgFile.h"

SvgFile::SvgFile()
{
    
    getRoot()->setAttribute("width", 1920 );
    getRoot()->setAttribute("height",  1080 );
    
    getRoot()->setAttribute("stroke-width", 1.0);
    getRoot()->setAttribute("stroke", "#000000");
    getRoot()->setAttribute("fill", "none");
    
    addGroup("Screen 1");
    addRect("Slice 1", 10, 10, 400, 400, 0);
    addRect("Slice 2", 540, 320, 200, 300, 33);
  
}

SvgFile::~SvgFile()
{
    svgData = nullptr;
}

bool SvgFile::save( File filename )
{
    if ( filename != File() )
    {
        //create the file if it doesn't exist
        if ( !filename.existsAsFile() )
            filename.create();
        
        return getRoot()->writeToFile( filename, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">");
    }
    
    //we were passed an invalid file, so we couldn't save
    DBG("Filename was invalid!");
    return false;
}



XmlElement* SvgFile::getRoot()
{
    if ( !svgData )
        svgData = new XmlElement("svg");
    
    //create the header data
    svgData->setAttribute("version", "1.1");
    svgData->setAttribute("xmlns","http://www.w3.org/2000/svg");
    
    return svgData;
}

XmlElement* SvgFile::getActiveElement()
{
    if ( !activeElement )
        return getRoot();
    else
        return activeElement;
}

void SvgFile::addGroup( String groupName )
{
    XmlElement* group = new XmlElement("g");
    group->setAttribute("id", groupName);
    getRoot()->addChildElement( group );
    activeElement = group;
}

void SvgFile::setStrokeWidth(float value)
{
    getActiveElement()->setAttribute("stroke-width", value);
    getActiveElement()->setAttribute("stroke", "#000000");
    getActiveElement()->setAttribute("fill", "none");
}

void SvgFile::addRect(String name, float x, float y, float w, float h, float rotation)
{
    XmlElement* rect = new XmlElement("rect");
    rect->setAttribute("id", name );
    rect->setAttribute("x", x);
    rect->setAttribute("y", y);
    rect->setAttribute("width", w);
    rect->setAttribute("height", h);
    rect->setAttribute("transform", "rotate("+String(rotation)+")");
    
    getActiveElement()->addChildElement( rect );
}


