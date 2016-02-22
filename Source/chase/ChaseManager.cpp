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
	
	currentSequence = -1;
	skipToNextSequence();
	
//	//fill the first sequences with 16 steps
//	sequenceMap[0][15] = SliceIndexArray{};
}

ChaseManager::~ChaseManager()
{
	
}

void ChaseManager::setStep(int sequence, int step, SliceIndexArray activeSlices)
{
	sequenceMap[sequence][step] = activeSlices;
}

void ChaseManager::setStep(SliceIndexArray activeSlices)
{
	setStep ( currentSequence, currentStep, activeSlices );
}

SliceIndexArray ChaseManager::getStep(int sequence, int step)
{
	return sequenceMap[sequence][step];
}

SliceIndexArray ChaseManager::getStep()
{
	return getStep( currentSequence, currentStep );
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

void ChaseManager::fillSequence()
{
	//make sure the sequence is filled
	if ( sequenceMap[currentSequence].size() == 0)
		sequenceMap[currentSequence][15] = SliceIndexArray{};
}

void ChaseManager::skipToSequence(int i)
{
	currentSequence = i;
	
	fillSequence();
	
	//when skipping sequences, the current step always resets
	skipToStep( 0 );
}

int ChaseManager::skipToNextSequence()
{
	currentSequence++;
	
	fillSequence();
	
	//when skipping sequences, the current step always resets
	skipToStep( 0 );
	
	return currentSequence;
}

int ChaseManager::skipToPreviousSequence()
{
	currentSequence--;
	if ( currentSequence < 0 && !sequenceMap.empty())
		currentSequence = sequenceMap.rbegin()->first;
	
	fillSequence();
	
	//when skipping sequences, the current step always resets
	skipToStep( 0 );

	return currentSequence;
}

void ChaseManager::skipToStep(int i)
{
	currentStep = i;
}

int ChaseManager::skipToNextStep()
{
	currentStep++;
	if ( currentStep > getLastStep() )
		currentStep = 0;
	return currentStep;
}

int ChaseManager::skipToPreviousStep()
{
	currentStep--;
	if ( currentStep < 0  )
		currentStep = getLastStep();
	
	return currentStep;
}

int ChaseManager::addStep()
{
	int lastStep = getLastStep();
	sequenceMap[currentSequence][lastStep + 1] = SliceIndexArray{};
	
	return getLastStep();

}

int ChaseManager::removeStep()
{
	//as long as we still have more than 1 step
	if ( sequenceMap[currentSequence].size() > 1 )
	{
		//make sure the second to last step exists
		if(sequenceMap[currentSequence][getLastStep()-1].size() == 0)
			sequenceMap[currentSequence][getLastStep()-1] = SliceIndexArray{};
		//delete the last one
		sequenceMap[currentSequence].erase( std::prev( sequenceMap[currentSequence].end() ) );
	}
	
	return getLastStep();
}

int ChaseManager::getLastStep()
{
	return sequenceMap[currentSequence].rbegin()->first;
}

int ChaseManager::getCurrentStep()
{
	return currentStep;
}

int ChaseManager::getCurrentSequence()
{
	return currentSequence;
}
