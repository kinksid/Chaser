//
//  SvgFile.h
//  Chaser
//
//  Created by Joris on 12/01/16.
//
//

#ifndef SvgFile_h
#define SvgFile_h

#include "JuceHeader.h"
#include "Slice.h"


class SvgFile
{
public:
    SvgFile();
    ~SvgFile();
    
    void createSvg( OwnedArray<Slice>& slices, int width, int height );
    bool save( File filename );
    
    

    
    ///void setStrokeColor(float r, float g, float b);
    void setStrokeWidth(float value );
    
private:
    ScopedPointer<XmlElement> svgData = nullptr;
    
    void addGroup ( String groupname );
    void addRect(String name, float x, float y, float w, float h, float rotation);

    XmlElement* getRoot();
    XmlElement* getActiveElement();
    XmlElement* activeElement;
    
};

#endif /* SvgFile_h */
