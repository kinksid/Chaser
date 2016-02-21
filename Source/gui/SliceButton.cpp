/*
  ==============================================================================

    SliceButton.cpp
    Created: 28 Dec 2014 11:15:33am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceButton.h"


SliceButton::SliceButton(Slice& s, Point<int> scale) : ShapeButton ( s.name, Colours::transparentBlack, Colours::transparentBlack, Colours::transparentBlack ), slice(s)
{
	setButtonText( s.name );
	//enable loads the state from the ass xml file
	//it can still be edited via the slicelist
	setVisible( s.enabled );
	name = s.name;
	createPath( scale );
	setClickingTogglesState( true );
	setToggleState( false, sendNotification );
}

SliceButton::~SliceButton()
{
    
}

void SliceButton::createPath( Point<int> scale )
{
	
	//check if the slice has maskpoints,
	//if so, it's a regular slice with a mask or a polyslice
	//and the mask points should be used for the path
	//otherwise it's a regular slice and the inputrect points should be used for the path
	if ( slice.maskPoints.size() > 0 )
	{
		path = makePath(slice.maskPoints, scale);
	}
	else
	{
		path = makePath(slice.inputRectPoints, scale);
	}
}

Path SliceButton::makePath( Array<Point<float>>& points, Point<int> scale )
{
	

	
	
	Path newPath;
	for ( int i = 0; i < points.size(); i++ )
	{
		Point<float> p = points[i];
		//points are stored normalized in 0...1 range
		//here points are transformed to fit within the preview window
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
	return path;
}

bool SliceButton::hitTest(int x, int y)
{
	//the path points are absolute, while the hitTest x and y points are relative to the bounds
	//that's why I'm adding getPosition to get the correct result
	if ( getPath().contains( float(x + getPosition().x), float(y + getPosition().y)) )
		return true;
	return false;
}

void SliceButton::paintButton(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
	if (isVisible())
	{
		Rectangle<float> r = getLocalBounds().toFloat().reduced ( 0.5f );
		
		if (isButtonDown)
		{
			const float sizeReductionWhenPressed = 0.01f;
			
			r = r.reduced (sizeReductionWhenPressed * r.getWidth(),
						   sizeReductionWhenPressed * r.getHeight());
		}
		const AffineTransform trans(getPath().getTransformToScaleToFit (r, false));
		
		ColourLookAndFeel claf;
		Colour col = getToggleState() ? claf.primaryColour : claf.backgroundColour;
		if ( isMouseOverButton )
			col = col.brighter(0.2f);
		g.setColour ( col );
		
		Path scaledToPreview = getPath();
		g.fillPath ( scaledToPreview, trans);
		
		g.setColour (claf.outlineColour);
		g.strokePath( scaledToPreview, PathStrokeType (1.0), trans);
	}

}

void SliceButton::resized()
{
	createPath( Point<int>(getParentWidth(), getParentHeight() ));
	Rectangle<int> bounds = path.getBounds().toType<int>();
	setBounds( bounds );
	
	//update the visibilty
	setVisible( slice.enabled && slice.screenIsUncollapsed );
}




