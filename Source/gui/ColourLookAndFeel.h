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
	
	Colour outlineColour = Colours::white;//Colour::fromRGB(0, 161, 255);
	Colour backgroundColour = Colour::fromRGB(25, 171, 255).darker(0.2f);
	Colour primaryColour = Colour::fromRGB(255, 146, 0);
	Colour textColour = Colours::whitesmoke;
	Colour textOnColour = Colours::whitesmoke;
	
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
	
	void drawPropertyPanelSectionHeader (Graphics& g, const String& name,
														 bool isOpen, int width, int height)
	{
		g.setColour (primaryColour);
		g.fillRect (0, 0, width, height);
		
		const float buttonSize = height * 0.75f;
		const float buttonIndent = (height - buttonSize) * 0.5f;
		
		drawTreeviewPlusMinusBox (g, Rectangle<float> (buttonIndent, buttonIndent, buttonSize, buttonSize), Colours::white, isOpen, false);
		
		const int textX = (int) (buttonIndent * 2.0f + buttonSize + 2.0f);
		
		g.setColour (Colours::black);
		g.setFont (Font (height * 0.7f, Font::bold));
		g.drawText (name, textX, 0, width - textX - 4, height, Justification::centredLeft, true);
	}
	
	void drawPropertyComponentBackground (Graphics&, int, int, PropertyComponent& )
	{
		//not drawing any background for the tree view
	}
	
	Rectangle<int> getPropertyComponentContentPosition (PropertyComponent& component)
	{
		return Rectangle<int> (component.getLocalBounds());
	}
	
	void drawPropertyComponentLabel (Graphics&, int, int, PropertyComponent& )
	{
		//not drawing a label for the treeview
	}
	
	void drawToggleButton (Graphics& g, ToggleButton& button,
										   bool, bool )
	{
		float fontSize = jmin (15.0f, button.getHeight() * 0.75f);
		const float tickWidth = fontSize * 1.1f;
		
		g.setColour ( textColour );
		
		Path p;
		p.addRoundedRectangle(tickWidth * 0.1f, tickWidth * 0.25f, tickWidth, tickWidth, 2);
		g.strokePath ( p, PathStrokeType(1.0f));

		if ( button.getToggleState() )
		{
			Path pip;
			pip.addRoundedRectangle(tickWidth * 0.2f, tickWidth * 0.35f, tickWidth * 0.8f, tickWidth * 0.8f, 2);
			g.fillPath(pip);
		}
		
		
		g.setFont (fontSize);
		
		if (! button.isEnabled())
			g.setOpacity (0.5f);
		
		const int textX = (int) tickWidth + 5;
		
		if ( button.getWidth() - textX - 2 > 0 )
		{
			g.drawFittedText (button.getButtonText(),
							  textX, 0,
							  button.getWidth() - textX - 2, button.getHeight(),
							  Justification::centredLeft, 10);
		}
	}


	
	//Colour::fromRGB(0, 113, 255);
	//178, 102, 0
	
};



#endif  // COLOURLOOKANDFEEL_H_INCLUDED
