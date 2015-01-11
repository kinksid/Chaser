/*
  ==============================================================================

    XmlSequence.cpp
    Created: 11 Jan 2015 12:04:53am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "XmlSequence.h"
/*
XmlElement animalsList ("ANIMALS");

for (int i = 0; i < numAnimals; ++i)
{
    // create an inner element..
    XmlElement* giraffe = new XmlElement ("GIRAFFE");
    
    giraffe->setAttribute ("name", "nigel");
    giraffe->setAttribute ("age", 10);
    giraffe->setAttribute ("friendly", true);
    
    // ..and add our new element to the parent node
    animalsList.addChildElement (giraffe);
}

// now we can turn the whole thing into a text document..
String myXmlDoc = animalsList.createDocument (String::empty);
@endcode
 
 */

XmlSequence::XmlSequence()
{
    sequence = new XmlElement ("sequence");
    
    for ( int i = 0; i < 16; i++ )
    {
        XmlElement* step = new XmlElement ("step");
        steps.add( step );
    }
}

XmlSequence::~XmlSequence()
{
    sequence = nullptr;
    steps.clear();
}

void XmlSequence::setStep(int step, Array<int> activeSlices)
{
    steps[step]->deleteAllChildElements();
    for ( int i = 0; i < activeSlices.size(); i++ )
    {
        DBG("Added to XML: "+ String(i));
        XmlElement* slice = new XmlElement( String (activeSlices[i]) );
        steps[step]->addChildElement(slice);
    }
}

Array<int> XmlSequence::getStep(int step)
{
    Array<int> activeSlices;
    for ( int i = 0; i < steps[step]->getNumChildElements(); i++ )
    {
        DBG("Read from XML: " + String(steps[step]->getChildElement(i)->getTagName().getIntValue()));
        activeSlices.add( steps[step]->getChildElement(i)->getTagName().getIntValue() );
    }
    return activeSlices;
}