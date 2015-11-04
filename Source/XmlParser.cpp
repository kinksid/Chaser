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
		if ( presetNode->hasTagName("screen") )
		{
			forEachXmlChildElement( *presetNode, screenNode )
			{
				if ( screenNode->hasTagName("slices") )
				{
					//clear the slice array so we don't get doubles
					slices.clear();
					
					forEachXmlChildElement( *screenNode, slicesNode)
					{
						if ( slicesNode->hasTagName("Slice") )
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
							
							//type 0 means only slices are loaded, no masks or crops
							if ( type == 0 )
							{
								Slice* slice = new Slice( name, enabled );
								//create 4 points out of the ltrb data
								for (int i = 0; i < 4; i++ )
								{
									Point<float> newPoint;
									switch (i)
									{
										case 0:
											newPoint = Point<float>(l,t);
											break;
										case 1:
											newPoint = Point<float>(r,t);
											break;
										case 2:
											newPoint = Point<float>(r,b);
											break;
										case 3:
											newPoint = Point<float>(l,b);
											break;
									}
									slice->inputRectPoints.add(newPoint);
								}
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

bool XmlParser::parseRes5Xml(XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices, Point<int>& resolution )
{
    slices.clear();
    
    XmlElement* screenSetup = xmlTreeToParse.getChildByName("ScreenSetup");
    if ( screenSetup != nullptr )
    {
		XmlElement* sizing = screenSetup->getChildByName("sizing");
		if ( sizing!= nullptr )
		{
			forEachXmlChildElement( *sizing, inputs )
			{
				if ( inputs->hasTagName("inputs") )
				{
					forEachXmlChildElement( *inputs, InputSize )
					{
						if ( InputSize->getStringAttribute("name") == "0:1" )
						{
							resolution.x = InputSize->getIntAttribute("width", 1920);
							resolution.y = InputSize->getIntAttribute("height", 1080);
						}
					}
				}
			}
		}
        XmlElement* screens = screenSetup->getChildByName("screens");
        if ( screens!= nullptr )
        {
            forEachXmlChildElement( *screens, child )
            {
                if ( child->hasTagName("Screen") )
                {
					if ( !child->getStringAttribute("name").contains("Scaling") )
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
											if ( child->hasTagName("Param") && child->getStringAttribute("name") == "Enabled")
											{
												newSlice->enabled = bool(child->getStringAttribute("value", "1").getIntValue());
											}
										}
									}
									XmlElement* inputRect = child->getChildByName("InputRect");
									if ( inputRect != nullptr )
									{
										newSlice->inputRectOrientation = inputRect->getStringAttribute("orientation", "0").getFloatValue();
										forEachXmlChildElement( *inputRect, child )
										{
											if ( child->hasTagName("v") )
											{
												Point<float> newPoint;
												addPointToSlice(newPoint, child, newSlice->inputRectPoints, resolution);
											}
										}
									}
									XmlElement* sliceMask = child->getChildByName("SliceMask");
									if ( sliceMask != nullptr )
									{
										XmlElement* shapeObject = sliceMask->getChildByName("ShapeObject");
										if ( shapeObject != nullptr )
										{
											XmlElement* maskRect = shapeObject->getChildByName("Rect");
											if ( maskRect != nullptr )
											{
												newSlice->maskRectOrientation = maskRect->getStringAttribute("orientation", "0").getFloatValue();
												forEachXmlChildElement( *maskRect, child )
												{
													if ( child->hasTagName("v") )
													{
														Point<float> newPoint;
														addPointToSlice(newPoint, child, newSlice->maskRectPoints, resolution);
													}
												}
											}
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
																addPointToSlice(newPoint, child, newSlice->maskPoints, resolution);
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
													addPointToSlice(newPoint, child, newSlice->maskPoints, resolution);
												}
											}
										}
									}
									
									slices.insert(0, newSlice);
									//slices.add( newSlice );
								}
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

void XmlParser::addPointToSlice(Point<float> newPoint, juce::XmlElement *element, Array<Point<float>>& pointType, Point<int> resolution)
{
	//TODO get these values from a popup when you open an ass 5 file
	newPoint.x = element->getStringAttribute("x", "0.0").getFloatValue() / float(resolution.x);
	newPoint.y = element->getStringAttribute("y", "0.0").getFloatValue() / float(resolution.y);
	pointType.add( newPoint );
}