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
	
	
	Slice( String n, bool enable) : name ( n ), enabled ( enable )
    {
		inputRectPoints.clear();
		maskPoints.clear();
		maskRectPoints.clear();
	}
	
	Slice ( const Slice& slice) : name (slice.name ), enabled(slice.enabled), inputRectPoints(slice.inputRectPoints), maskPoints(slice.maskPoints), maskRectPoints(slice.maskRectPoints)
	{
        
	}
    
    Slice(){}
	
    ~Slice()
    {
        
    }

    struct vec4f
    {
        vec4f( float x, float y, float z, float w ): x(0.0), y(0.0), z(0.0), w(0.0){}
        vec4f() : x(0.0), y(0.0), z(0.0), w(0.0){}
        float x;
        float y;
        float z;
        float w;
    };
    
    String name;
	bool enabled;
	
    Array<Point<float>> inputRectPoints;
    Array<Point<float>> maskPoints;
	Array<Point<float>> maskRectPoints;
	
    
private:

};



#endif  // SLICE_H_INCLUDED
