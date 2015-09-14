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
    
	
    setToggleState( false, sendNotification );
    
    proportionalX = l;
    proportionalY = t;
    proportionalW = r - l;
    proportionalH = b - t;

}
																																		  


SliceButton::SliceButton(Slice* s, Point<int> scale) : SliceButton(s->name, s->enabled, s->proportionalX, s->proportionalY, s->proportionalW + s->proportionalX, s->proportionalH + s->proportionalY)
{
	DBG(s->name);
	path = makePath(s->inputRectPoints, scale);
	mask = makePath(s->maskPoints, scale);
	maskRect = makePath(s->maskRectPoints, scale);

	Rectangle<int> bounds = maskRect.isEmpty()? path.getBounds().toType<int>() : maskRect.getBounds().toType<int>();
	setBounds( bounds );
	//set the path so that it starts at (0,0) of its bounds
	getPath().applyTransform(AffineTransform().translated(getPosition()));
	setClickingTogglesState( true );
}

SliceButton::~SliceButton()
{
    
}

Path SliceButton::makePath(Array<Point<float> > points, Point<int> scale )
{
	Path newPath;
	
	for ( int i = 0; i < points.size(); i++ )
	{
		Point<float> p = points[i];
		p *= scale;
		if ( i == 0)
		{
			newPath.startNewSubPath( p );
			
		}
		else
			newPath.lineTo( p );
	}
	newPath.closeSubPath();
	
	return newPath;
}

Path SliceButton::getPath()
{
	if ( mask.isEmpty())
		return path;
	else
		return mask;
}

bool SliceButton::hitTest(int x, int y)
{
	if ( getPath().contains( x + getPosition().x, y + getPosition().y) )
		return true;
	return false;
}

void SliceButton::paintButton(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
	if (isEnabled())
	{
		Rectangle<float> r = getLocalBounds().toFloat().reduced ( 0.5f);
		if (isButtonDown)
		{
			const float sizeReductionWhenPressed = 0.01f;
			
			r = r.reduced (sizeReductionWhenPressed * r.getWidth(),
						   sizeReductionWhenPressed * r.getHeight());
		}
		const AffineTransform trans (getPath().getTransformToScaleToFit (r, true));
		
		ColourLookAndFeel claf;
		Colour col = getToggleState() ? claf.primaryColour : claf.backgroundColour;
		if ( isMouseOverButton )
			col = col.brighter(0.2);
		g.setColour ( col );
		
		g.fillPath ( getPath(), trans);
		
		g.setColour (claf.outlineColour);
		g.strokePath( getPath(), PathStrokeType (1.0), trans);
	}
	
}




