/*
  ==============================================================================

    SliceButton.cpp
    Created: 28 Dec 2014 11:15:33am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceButton.h"

//==============================================================================
SliceButton::SliceButton(String n, bool enable, double l, double t, double r, double b) : ShapeButton ( n, Colours::transparentBlack, Colours::transparentBlack, Colours::transparentBlack )
{
    setButtonText( n );
    
    //enable loads the state from the ass xml file
    //it can still be edited via the slicelist
    enabled = enable;
    
    setClickingTogglesState( true );
    setToggleState( false, sendNotification );
    
    proportionalX = l;
    proportionalY = t;
    proportionalW = r - l;
    proportionalH = b - t;

}
																																		  


SliceButton::SliceButton(Slice* s, Point<int> scale) : SliceButton(s->name, s->enabled, s->proportionalX, s->proportionalY, s->proportionalW + s->proportionalX, s->proportionalH + s->proportionalY)
{
	
	for ( int i = 0; i < s->inputRectPoints.size(); i++ )
	{
		Point<float> p = s->inputRectPoints[i];
		p.x /= 1920.0;
		p.y /= 1080.0;
		p *= scale;		
		if ( i == 0)
		{
			path.startNewSubPath( p );
			
		}
		else
			path.lineTo( p );
		
	}
	path.closeSubPath();
	
	for ( int i = 0; i < s->maskPoints.size(); i++ )
	{
		Point<float> p = s->maskPoints[i];
		p.x /= 1920.0;
		p.y /= 1080.0;
		p *= scale;
		if ( i == 0)
		{
			mask.startNewSubPath( p );
			
		}
		else
			mask.lineTo( p );
		
		//TODO maskClippedLine?
		
	}
	mask.closeSubPath();
	
	for ( int i = 0; i < s->maskRectPoints.size(); i++ )
	{
		Point<float> p = s->maskRectPoints[i];
		p.x /= 1920.0;
		p.y /= 1080.0;
		p *= scale;
		if ( i == 0)
		{
			maskRect.startNewSubPath( p );
			
		}
		else
			maskRect.lineTo( p );
		
	}
	maskRect.closeSubPath();
	
	
	setShape(getPath(), true, true, false);
	Rectangle<int> bounds = maskRect.isEmpty()? path.getBounds().toType<int>() : maskRect.getBounds().toType<int>();
	setBounds( bounds );
	setOutline(Colours::white, 1.0);
}

SliceButton::~SliceButton()
{
    
}




