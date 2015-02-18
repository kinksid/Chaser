/*
  ==============================================================================

    ColourLookAndFeel.h
    Created: 16 Feb 2015 7:59:37pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef COLOURLOOKANDFEEL_H_INCLUDED
#define COLOURLOOKANDFEEL_H_INCLUDED

class ColourLookAndFeel : public LookAndFeel_V1
{
public:
	ColourLookAndFeel()
	{
		setColour(TextButton::buttonOnColourId, outlineColour);
	}
	~ColourLookAndFeel(){}
	
	
	
	void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
							   bool isMouseOverButton, bool isButtonDown)
	{
		const int width = button.getWidth();
		const int height = button.getHeight();
		
		const float indent = 2.0f;
		const int cornerSize = jmin (roundToInt (width * 0.4f),
									 roundToInt (height * 0.4f));
		
		Path p;
		p.addRoundedRectangle (indent, indent,
							   width - indent * 2.0f,
							   height - indent * 2.0f,
							   (float) cornerSize);
		
		Colour bc (backgroundColour.withMultipliedSaturation (0.3f));
		
		if (isMouseOverButton)
		{
			if (isButtonDown)
				bc = bc.brighter();
			else if (bc.getBrightness() > 0.5f)
				bc = bc.darker (0.1f);
			else
				bc = bc.brighter (0.1f);
		}
		
		g.setColour (bc);
		g.fillPath (p);
		
		g.setColour (outlineColour.withAlpha ((isMouseOverButton) ? 1.0f : 0.8f));
		g.strokePath (p, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.4f));
	}
	
	Colour getOutlineColour ()
	{
		return outlineColour;
	}
	
private:
	Colour outlineColour = Colours::khaki;
	
};



#endif  // COLOURLOOKANDFEEL_H_INCLUDED
