/*
  ==============================================================================

    ChaseManager.cpp
    Created: 21 Feb 2016 5:04:36pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "ChaseManager.h"

ChaseManager::ChaseManager()
{
	setName( "Default Chaser" );
}

ChaseManager::~ChaseManager()
{
	
}

void ChaseManager::setStep(int sequence, int step, SliceArray activeSlices)
{
	sequenceMap[sequence][step] = activeSlices;
}

SliceArray ChaseManager::getStep(int sequence, int step)
{
	return sequenceMap[sequence][step];
}

void ChaseManager::clearAll()
{
	//TODO not quite sure if this will leak Slice*
	sequenceMap.clear();
}

void ChaseManager::setName(juce::String name)
{
	chaserName = name;
}

String ChaseManager::getName()
{
	return chaserName;
}