/*
  ==============================================================================

    Slice.h
    Created: 10 Jan 2015 7:46:04pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef SLICE_H_INCLUDED
#define SLICE_H_INCLUDED

//Quick class that holds a slice with all the relevant info

class Slice
{
public:
	
	
	Slice( String n, bool enable, double l, double t, double r, double b ) : name ( n ), enabled ( enable ), proportionalX ( l ), proportionalY ( t ), proportionalW ( r - l ), proportionalH ( b - t )
    {}
	
	Slice ( const Slice& slice) : name (slice.name ), enabled(slice.enabled), proportionalX(slice.proportionalX), proportionalY(slice.proportionalY), proportionalW(slice.proportionalW), proportionalH(slice.proportionalH)
	{
		
	}
	
    Slice(){}
    ~Slice()
    {
        
    }
    
    String name;
	bool enabled;
    double proportionalX;
    double proportionalY;
    double proportionalW;
    double proportionalH;
	
    
private:
	OwnedArray<Point<float>> inputRectPoints;
	OwnedArray<Point<float>> inputMaskPoints;
};



#endif  // SLICE_H_INCLUDED
