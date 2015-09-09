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