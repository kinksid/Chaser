/*
  ==============================================================================

    SliceManager.cpp
    Created: 20 Feb 2016 6:11:01pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "SliceManager.h"

SliceManager::SliceManager()
{
	resolution = Point<int>(1920,1080);
}

SliceManager::~SliceManager()
{
	slices.clear();
}

//void SliceManager::addSlices(Array<Slice *> sliceArray)
//{
//	slices.addArray(sliceArray);
//}

OwnedArray<Slice>& SliceManager::getSlices()
{
	return slices;
}

Point<int>& SliceManager::getResolution()
{
	return resolution;
}