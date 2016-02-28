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
	
	currentSequence = 15;
	fillSequence();
	currentSequence = 0;
	fillSequence();
	
//	//set the default max sequence with the default max steps
//	sequenceMap[15][15] = SliceIndexArray{};
}

ChaseManager::~ChaseManager()
{
	
}

void ChaseManager::setStep(int sequence, int step, SliceIndexArray activeSlices)
{
	sequenceMap[sequence][step] = activeSlices;
}

void ChaseManager::setCurrentStep(SliceIndexArray activeSlices)
{
	setStep ( currentSequence, currentStep, activeSlices );
}

SliceIndexArray ChaseManager::getStepSlices(int sequence, int step)
{
	return sequenceMap[sequence][step];
}

SliceIndexArray ChaseManager::getCurrentStepSlices()
{
	return getStepSlices( currentSequence, currentStep );
}

void ChaseManager::clearAll()
{
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
	//make sure the sequence is empty
	if ( sequenceMap[currentSequence].size() == 0)
	{
		sequenceMap[currentSequence][15] = SliceIndexArray{};
		nameMap[currentSequence] = "Sequence " + String (currentSequence + 1);
	}
}

void ChaseManager::skipToSequence(int i)
{
	currentSequence = i;
	
	if ( currentSequence > getLastSequenceIndex() )
		currentSequence = getLastSequenceIndex();
	
	//make sure the sequence is filled
	fillSequence();
	
	//when skipping sequences, the current step always resets
	skipToStep( 0 );
}

int ChaseManager::skipToNextSequence()
{
	currentSequence++;
	
	if ( currentSequence > getLastSequenceIndex() )
		currentSequence = 0;
	
	//make sure the sequence is filled
	fillSequence();
	
	//when skipping sequences, the current step always resets
	skipToStep( 0 );
	
	return currentSequence;
}

int ChaseManager::skipToPreviousSequence()
{
	currentSequence--;
	if ( currentSequence < 0 )
		currentSequence = getLastSequenceIndex();
	
	//make sure the sequence is filled
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
	
	if ( currentStep > getLastStepIndex() )
		currentStep = 0;
	
	return currentStep;
}

int ChaseManager::skipToPreviousStep()
{
	currentStep--;
	
	if ( currentStep < 0  )
		currentStep = getLastStepIndex();
	
	return currentStep;
}

int ChaseManager::addStep()
{
	int lastStep = getLastStepIndex();
	sequenceMap[currentSequence][lastStep + 1] = SliceIndexArray{};
	
	return getLastStepIndex();
}

int ChaseManager::removeStep()
{
	//only remove as long as we still have more than 1 step
	if ( sequenceMap[currentSequence].size() > 1 )
	{
		//make sure the second to last step exists
		if(sequenceMap[currentSequence][getLastStepIndex()-1].size() == 0)
			sequenceMap[currentSequence][getLastStepIndex()-1] = SliceIndexArray{};
		
		//delete the last one
		sequenceMap[currentSequence].erase( std::prev( sequenceMap[currentSequence].end() ) );
	}
	
	return getLastStepIndex();
}

int ChaseManager::getLastStepIndex()
{
	return sequenceMap[currentSequence].rbegin()->first;
}

int ChaseManager::getLastSequenceIndex()
{
	return sequenceMap.rbegin()->first;
}

int ChaseManager::getCurrentStepIndex()
{
	return currentStep;
}

int ChaseManager::getCurrentSequenceIndex()
{
	return currentSequence;
}

String ChaseManager::getCurrentSequenceName()
{
	return nameMap[currentSequence];
}

void ChaseManager::setCurrentSequenceName(juce::String newName)
{
	nameMap[currentSequence] = newName;
}
