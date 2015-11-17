/*
  ==============================================================================

    XmlParser.cpp
    Created: 9 Sep 2015 8:45:41pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "XmlParser.h"

bool XmlParser::parseRes4ConfigXml(juce::XmlElement &xmlTreeToParse, OwnedArray<Slice>& slices, Point<int> &resolution)
{
	forEachXmlChildElement( xmlTreeToParse, avenueChild )
	{
		if ( avenueChild->hasTagName("settings") )
		{
			forEachXmlChildElement( *avenueChild, settingsChild )
			{
				if ( settingsChild->hasTagName("video"))
				{
					forEachXmlChildElement( *settingsChild, videoChild )
					{
						if ( videoChild->hasTagName("settings"))
						{
							forEachXmlChildElement( *videoChild, videoSettingsChild )
							{
								if ( videoSettingsChild->hasTagName("screenSetup"))
								{
									forEachXmlChildElement( *videoSettingsChild, screenSetupChild )
									{
										if ( screenSetupChild->hasTagName("screens"))
										{
											forEachXmlChildElement(*screenSetupChild, screensChild )
											{
												if ( screensChild->hasTagName("advancedScreens") )
												{
													forEachXmlChildElement( *screensChild, advancedScreensChild )
													{
														if ( advancedScreensChild->hasTagName("screen") )
														{
															forEachXmlChildElement( *advancedScreensChild, screenNode )
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
																			int type = 0;
																			float l = 0.0f;
																			float t = 0.0f;
																			float r = 0.0f;
																			float b = 0.0f;
																			
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
																					l = float(sliceNode->getDoubleAttribute("l"));
																					t = float(sliceNode->getDoubleAttribute("t"));
																					r = float(sliceNode->getDoubleAttribute("r"));
																					b = float(sliceNode->getDoubleAttribute("b"));
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
																				slices.insert(0, slice);
																			}
																		}						
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				if ( settingsChild->hasTagName("composition"))
				{
					//read out the resolution from the comp file
					File compFile = settingsChild->getStringAttribute("startupFileName");
					if (compFile.exists())
					{
						ScopedPointer<XmlElement> composition;
						composition = (XmlDocument::parse ( compFile ));
						forEachXmlChildElement( *composition, compChild )
						{
							if ( compChild->hasTagName("generalInfo"))
							{
								resolution.x = compChild->getIntAttribute("width", 1920);
								resolution.y = compChild->getIntAttribute("height", 1080);
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

bool XmlParser::parseRes4Xml(XmlElement& xmlTreeToParse, OwnedArray<Slice>& slices, Point<int>& resolution)
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
							int type = 0;
							float l = 0.0f;
							float t = 0.0f;
							float r = 0.0f;
							float b = 0.0f;
							
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
									l = float(sliceNode->getDoubleAttribute("l"));
									t = float(sliceNode->getDoubleAttribute("t"));
									r = float(sliceNode->getDoubleAttribute("r"));
									b = float(sliceNode->getDoubleAttribute("b"));
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
								slices.insert(0, slice);
							}
						}						
					}
				}
			}
		}
	}
	
	//try to get the resolution from the config.xml file and the associated comp file
	File advancedLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Resolume Arena 4/preferences/config.xml";
	if ( advancedLocation.exists() )
	{
		ScopedPointer<XmlElement> xmlRoot;
		xmlRoot = (XmlDocument::parse ( advancedLocation ));
		forEachXmlChildElement( *xmlRoot, avenueChild )
		{
			if ( avenueChild->hasTagName("settings") )
			{
				forEachXmlChildElement( *avenueChild, settingsChild )
				{
					if ( settingsChild->hasTagName("composition"))
					{
						//read out the resolution from the comp file
						File compFile = settingsChild->getStringAttribute("startupFileName");
						if (compFile.exists())
						{
							ScopedPointer<XmlElement> composition;
							composition = (XmlDocument::parse ( compFile ));
							forEachXmlChildElement( *composition, compChild )
							{
								if ( compChild->hasTagName("generalInfo"))
								{
									resolution.x = compChild->getIntAttribute("width", 1920);
									resolution.y = compChild->getIntAttribute("height", 1080);
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

String XmlParser::getAdvancedPresetNameFromRes5Xml( XmlElement& xmlTreeToParse )
{
	String fileNameToReturn = String().empty;

	if ( xmlTreeToParse.hasAttribute("presetFile"))
		fileNameToReturn = xmlTreeToParse.getStringAttribute("presetFile", String().empty);

	return fileNameToReturn;
}

bool XmlParser::parseRes5PrefXml(juce::XmlElement& screenSetup, OwnedArray<Slice>& slices, Point<int>& resolution)
{
	slices.clear();

	
	XmlElement* sizing = screenSetup.getChildByName("sizing");
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
	XmlElement* screens = screenSetup.getChildByName("screens");
	if ( screens!= nullptr )
	{
		forEachXmlChildElement( *screens, child )
		{
			if ( child->hasTagName("Screen") )
			{
				if ( !child->getStringAttribute("name").containsIgnoreCase("scaling") )
				{
					XmlElement* layers = child->getChildByName("layers");
					if ( layers != nullptr )
					{
						forEachXmlChildElement( *layers, child )
						{
							if ( child->hasTagName("Slice") || child->hasTagName("Polygon"))
							{
								Slice* newSlice = new Slice();
								newSlice->enabled = true;
								
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
											newSlice->enabled = child->getStringAttribute("value", "1").getIntValue() != 0;
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
    XmlElement* screenSetup = xmlTreeToParse.getChildByName("ScreenSetup");
	
	slices.clear();
	
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
					if ( !child->getStringAttribute("name").containsIgnoreCase("scaling") )
					{
						XmlElement* layers = child->getChildByName("layers");
						if ( layers != nullptr )
						{
							forEachXmlChildElement( *layers, child )
							{
								if ( child->hasTagName("Slice") || child->hasTagName("Polygon"))
								{
									Slice* newSlice = new Slice();
									newSlice->enabled = true;
									
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
												newSlice->enabled = child->getStringAttribute("value", "1").getIntValue() != 0;
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

	newPoint.x = element->getStringAttribute("x", "0.0").getFloatValue() / float(resolution.x);
	newPoint.y = element->getStringAttribute("y", "0.0").getFloatValue() / float(resolution.y);
	pointType.add( newPoint );
}