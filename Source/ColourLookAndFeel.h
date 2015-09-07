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
		const int cornerSize = jmin (roundToInt (width * 0.1f),
									 roundToInt (height * 0.1f));
		
		Path p;
		p.addRoundedRectangle (indent, indent,
							   width - indent * 2.0f,
							   height - indent * 2.0f,
							   (float) cornerSize);
		
		Colour bc (backgroundColour);
		
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
		
		g.setColour (outlineColour.withMultipliedBrightness((isMouseOverButton) ? 1.2f : 1.0f));
		g.strokePath (p, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.0f));
	}

	Colour outlineColour = Colours::white;//Colour::fromRGB(0, 161, 255);
	Colour backgroundColour = Colour::fromRGB(25, 171, 255).darker(0.2f);
	Colour primaryColour = Colour::fromRGB(255, 146, 0);
	Colour textColour = Colours::whitesmoke;
	Colour textOnColour = Colours::whitesmoke;
	
	//Colour::fromRGB(0, 113, 255);
	//178, 102, 0
	
};



#endif  // COLOURLOOKANDFEEL_H_INCLUDED
