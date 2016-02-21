/*
  ==============================================================================

    Slice.h
    Created: 10 Jan 2015 7:46:04pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef SLICE_H_INCLUDED
#define SLICE_H_INCLUDED

#include "JuceHeader.h"

//Quick class that holds a slice with all the relevant info

class Slice
{
public:
	
	Slice( String n, bool enable) : name ( n ), enabled ( enable )
    {
		inputRectPoints.clear();
		maskPoints.clear();
		maskRectPoints.clear();
		inputRectOrientation = 0.0;
		maskRectOrientation = 0.0;
	}
	
	Slice ( const Slice& slice ) : name (slice.name ), enabled(slice.enabled), inputRectPoints(slice.inputRectPoints), maskPoints(slice.maskPoints), maskRectPoints(slice.maskRectPoints), inputRectOrientation(slice.inputRectOrientation), maskRectOrientation(slice.maskRectOrientation){	}
    
	Slice() : Slice ("New Slice", false ){}
	
	~Slice(){}
	
    String name;
	bool enabled;
	//when a screen is collapsed, the slice should not draw in preview
	//i cannot use the enabled bool for this, because when the screen is uncollapsed
	//this value should be maintained
	bool screenIsUncollapsed = true;
	
	//which screen and uniqued id this slice is a part of
	std::pair<int, String> screenPair = std::make_pair(0, "Default Screen");
	
    Array<Point<float>> inputRectPoints;
    Array<Point<float>> maskPoints;
	
	//currently the maskRectPoints are not being used for anything
	//i use the inputRect of the slice to decide which parts of the slice to draw
	Array<Point<float>> maskRectPoints;
	
	float inputRectOrientation;
	float maskRectOrientation;
    
private:

};



#endif  // SLICE_H_INCLUDED
