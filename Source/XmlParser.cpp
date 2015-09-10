/*
  ==============================================================================

    XmlParser.cpp
    Created: 9 Sep 2015 8:45:41pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "XmlParser.h"

bool XmlParser::parseRes4Xml(XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices)
{
	forEachXmlChildElement( xmlTreeToParse, presetNode )
	{
		if ( presetNode != nullptr && presetNode->hasTagName("screen") )
		{
			forEachXmlChildElement( *presetNode, screenNode )
			{
				if ( screenNode != nullptr && screenNode->hasTagName("slices") )
				{
					//clear the slice array so we don't get doubles
					slices.clear();
					
					forEachXmlChildElement( *screenNode, slicesNode)
					{
						if ( slicesNode != nullptr && slicesNode->hasTagName("Slice") )
						{
							String name;
							bool enabled = slicesNode->getBoolAttribute("isEnabled");
							int type;
							double l;
							double t;
							double r;
							double b;
							
							forEachXmlChildElement( *slicesNode, sliceNode)
							{
								if ( sliceNode != nullptr )
								{
									
									if ( sliceNode->hasTagName("name") )
									{
										name = sliceNode->getStringAttribute("value");
									}
									
									if ( sliceNode->hasTagName("type") )
									{
										type = sliceNode->getIntAttribute("value");
									}
									
									if ( sliceNode->hasTagName("rect") )
									{
										l = sliceNode->getDoubleAttribute( "l" );
										t = sliceNode->getDoubleAttribute( "t" );
										r = sliceNode->getDoubleAttribute( "r" );
										b = sliceNode->getDoubleAttribute( "b" );
									}
								}
							}
							
							//type 0 means only slices are loaded, no masks or crops
							if ( type == 0 )
							{
								Slice* slice = new Slice( name, enabled, l, t, r, b);
								slices.add( slice );
							}
						}						
					}
				}
			}
		}
	}
	
	if ( slices.size() == 0 )
	{
		DBG("Not able to parse any slice data");
		return false;
	}
	else
	{
		DBG("Slice data parsed succesfully");
		return true;
	}
}

bool XmlParser::parseRes5Xml(XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices)
{
   
    slices.clear();
    
    XmlElement* screenSetup = xmlTreeToParse.getChildByName("ScreenSetup");
    if ( screenSetup != nullptr )
    {
        XmlElement* screens = screenSetup->getChildByName("screens");
        if ( screens!= nullptr )
        {
            forEachXmlChildElement( *screens, child )
            {
                if ( child->hasTagName("Screen") )
                {
                    XmlElement* layers = child->getChildByName("layers");
                    if ( layers != nullptr )
                    {
                        forEachXmlChildElement( *layers, child )
                        {
                            if ( child->hasTagName("Slice") || child->hasTagName("Polygon"))
                            {
                                Slice* newSlice = new Slice();
                                
                                XmlElement* params = child->getChildByName("Params");
                                if ( params != nullptr )
                                {
                                    forEachXmlChildElement( *params, child )
                                    {
                                        if ( child->hasTagName("Param") && child->getStringAttribute("name") == "Name")
                                        {
                                            newSlice->name = child->getStringAttribute("value", "Slice");
                                        }
                                    }
                                }
                                XmlElement* inputRect = child->getChildByName("InputRect");
                                if ( inputRect != nullptr )
                                {
                                    forEachXmlChildElement( *inputRect, child )
                                    {
                                        if ( child->hasTagName("v") )
                                        {
                                            Point<float> newPoint;
                                            newPoint.x = child->getStringAttribute("x", "0.0").getFloatValue();
                                            newPoint.y = child->getStringAttribute("y", "0.0").getFloatValue();
                                            newSlice->inputRectPoints.add( newPoint );
                                        }
                                    }
                                }
                                XmlElement* sliceMask = child->getChildByName("SliceMask");
                                if ( sliceMask != nullptr )
                                {
                                    XmlElement* shapeObject = sliceMask->getChildByName("ShapeObject");
                                    if ( shapeObject != nullptr )
                                    {
                                        XmlElement* shape = shapeObject->getChildByName("Shape");
                                        if ( shape != nullptr )
                                        {
                                            XmlElement* contour = shape->getChildByName("Contour");
                                            if ( contour != nullptr )
                                            {
                                                XmlElement* points = contour->getChildByName("points");
                                                if ( points != nullptr )
                                                {
                                                    forEachXmlChildElement( *points, child )
                                                    {
                                                        if ( child->hasTagName("v") )
                                                        {
                                                            Point<float> newPoint;
                                                            newPoint.x = child->getStringAttribute("x", "0.0").getFloatValue();
                                                            newPoint.y = child->getStringAttribute("y", "0.0").getFloatValue();
                                                            newSlice->inputMaskPoints.add( newPoint );
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                XmlElement* contour = child->getChildByName("InputContour");
                                if ( contour != nullptr )
                                {
                                    XmlElement* points = contour->getChildByName("points");
                                    if ( points != nullptr )
                                    {
                                        forEachXmlChildElement( *points, child )
                                        {
                                            if ( child->hasTagName("v") )
                                            {
                                                Point<float> newPoint;
                                                newPoint.x = child->getStringAttribute("x", "0.0").getFloatValue();
                                                newPoint.y = child->getStringAttribute("y", "0.0").getFloatValue();
                                                newSlice->inputMaskPoints.add( newPoint );
                                            }
                                        }
                                    }
                                }
                                
                                slices.add( newSlice );
                            }
                        }
                    }
                }
            }
        }
    }
    
    
  
    
    
    
    if ( slices.size() == 0 )
    {
        DBG("Not able to parse any slice data");
        return false;
    }
    else
    {
        DBG("Slice data parsed succesfully");
        return true;
    }
}