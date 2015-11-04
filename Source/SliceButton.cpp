/*
  ==============================================================================

    SliceButton.cpp
    Created: 28 Dec 2014 11:15:33am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceButton.h"


SliceButton::SliceButton(Slice* s, Point<int> scale) : ShapeButton ( s->name, Colours::transparentBlack, Colours::transparentBlack, Colours::transparentBlack ), slice(s)
{
	setButtonText( s->name );
	//enable loads the state from the ass xml file
	//it can still be edited via the slicelist
	setVisible( s->enabled );
	name = s->name;
	
	createPath( scale );
	setClickingTogglesState( true );
	setToggleState( false, sendNotification );
}

SliceButton::~SliceButton()
{
    
}

void SliceButton::createPath( Point<int> scale )
{
	path = makePath(slice->inputRectPoints, scale);
	mask = makePath(slice->maskPoints, scale);
	maskRect = makePath(slice->maskRectPoints, scale);
	
	Rectangle<int> bounds = maskRect.isEmpty()? path.getBounds().toType<int>() : maskRect.getBounds().getSmallestIntegerContainer();
	setBounds( bounds );
	
	//set the path so that it starts at (0,0) of its bounds
	getPath().applyTransform(AffineTransform().translated(getPosition()));
}

Path SliceButton::makePath( Array<Point<float>>& points, Point<int> scale )
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
	if (isVisible())
	{
		Rectangle<float> r = getLocalBounds().toFloat().reduced ( 0.5f );
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
		
		//AffineTransform previewScale = AffineTransform::identity.scaled(0.5f,0.5f);
		Path scaledToPreview = getPath();
		//scaledToPreview.applyTransform( previewScale );
		g.fillPath ( scaledToPreview, trans);
		
		g.setColour (claf.outlineColour);
		g.strokePath( scaledToPreview, PathStrokeType (1.0), trans);
	}
	
}

void SliceButton::resized()
{
	createPath( Point<int>(getParentWidth(), getParentHeight() ));
}




