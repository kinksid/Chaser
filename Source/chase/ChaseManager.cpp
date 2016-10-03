/*
  ==============================================================================

  ChaseManager.cpp
  Created: 21 Feb 2016 5:04:36pm
  Author:  Joris de Jong

  ==============================================================================
  */

#include "ChaseManager.h"

ChaseManager::ChaseManager( ChaserXmlManager* xmlManager ) : xmlManager( xmlManager )
{
	chaserName = ("Default Chaser");

	currentSequence = 15;
	fillSequence();
	currentSequence = 0;
	fillSequence();

	currentStep = 0;
}

ChaseManager::~ChaseManager()
{

}

void ChaseManager::setStep( int sequence, int step, SliceIndexArray activeSlices )
{
	sequenceMap[ sequence ][ step ] = activeSlices;
	writeToXml();
}

void ChaseManager::setCurrentStep( SliceIndexArray activeSlices )
{
	setStep( currentSequence, getCurrentStepIndex(), activeSlices );
}

SliceIndexArray ChaseManager::getStepSlices( int sequence, int step )
{
	return sequenceMap[ sequence ][ step ];
}

SliceIndexArray ChaseManager::getCurrentStepSlices()
{
	return getStepSlices( currentSequence, getCurrentStepIndex() );
}

void ChaseManager::clearAll()
{
	sequenceMap.clear();
	writeToXml();
}

void ChaseManager::setName( juce::String name )
{
	chaserName = name;
	writeToXml();
}

String ChaseManager::getName()
{
	return chaserName;
}

void ChaseManager::fillSequence()
{
	//first make sure the sequence is empty
	//otherwise we can just leave it
	if ( sequenceMap[ currentSequence ].size() == 0 )
	{
		sequenceMap[ currentSequence ][ 15 ] = SliceIndexArray{};
		nameMap[ currentSequence ] = "Sequence " + String( currentSequence + 1 );
	}
}

void ChaseManager::skipToSequence( int i )
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

void ChaseManager::skipToStep( int i )
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

	if ( currentStep < 0 )
		currentStep = getLastStepIndex();

	return currentStep;
}

int ChaseManager::setStepCount( int i )
{
	while ( i > getLastStepIndex() + 1 )
		addStep();
	while ( i < getLastStepIndex() + 1 )
		removeStep();

	return getLastStepIndex();
}

int ChaseManager::addStep()
{
	int lastStep = getLastStepIndex();
	sequenceMap[ currentSequence ][ lastStep + 1 ] = SliceIndexArray{};

	writeToXml();

	return getLastStepIndex();
}

int ChaseManager::removeStep()
{
	//only remove as long as we still have more than 1 step
	if ( getLastStepIndex() > 0 )
	{
		//make sure the second to last step exists
		if ( sequenceMap[ currentSequence ][ getLastStepIndex() - 1 ].size() == 0 )
			sequenceMap[ currentSequence ][ getLastStepIndex() - 1 ] = SliceIndexArray{};

		//delete the last one
		//can't use std::prev because 10.7 can't use c++11
		StepMap::iterator it = sequenceMap[ currentSequence ].end();
		if ( it != sequenceMap[ currentSequence ].begin() )
		{
			it--;
			sequenceMap[ currentSequence ].erase( it );
		}
	}

	writeToXml();

	return getLastStepIndex();
}

int ChaseManager::getLastStepIndex()
{
	return sequenceMap[ currentSequence ].rbegin()->first;
}

int ChaseManager::getLastSequenceIndex()
{
	return sequenceMap.rbegin()->first;
}

int ChaseManager::getCurrentStepIndex()
{
	//check if we're still in bounds
	if ( currentStep > getLastStepIndex() )
		currentStep = getLastStepIndex();

	return currentStep;
}

int ChaseManager::getCurrentSequenceIndex()
{
	return currentSequence;
}

String ChaseManager::getCurrentSequenceName()
{
	return nameMap[ currentSequence ];
}

void ChaseManager::setCurrentSequenceName( juce::String newName )
{
	nameMap[ currentSequence ] = newName;

	writeToXml();
}

XmlElement* ChaseManager::getSequencesAsXml()
{
	XmlElement* sequencesXml = new XmlElement( "sequences" );

	//loop through all the sequences
	for ( auto sequence : sequenceMap )
	{
		//for every sequence, create an xmlelement and store the name
		XmlElement* sequenceXml = new XmlElement( "sequence" );
		sequenceXml->setAttribute( "nr", sequence.first );
		sequenceXml->setAttribute( "name", nameMap[ sequence.first ] );
		sequencesXml->addChildElement( sequenceXml );

		//then create an xmlelement to store the steps
		XmlElement* stepsXml = new XmlElement( "steps" );
		sequenceXml->addChildElement( stepsXml );

		//loop through this sequence's steps
		for ( auto step : sequence.second )
		{
			//for every step, create an xmlelement and store the step nr
			XmlElement* stepXml = new XmlElement( "step" );
			stepXml->setAttribute( "nr", step.first );
			stepsXml->addChildElement( stepXml );

			//loop through this step's active slices
			for ( int64 slice : step.second )
			{
				//for every active slice, create an xmlelement and store the slice nr
				XmlElement* sliceXml = new XmlElement( "slice" );
				sliceXml->setAttribute( "uniqueId", String( slice ) );
				stepXml->addChildElement( sliceXml );
			}
		}
	}

	return sequencesXml;
}

void ChaseManager::writeToXml()
{
	if ( xmlManager )
		xmlManager->saveXmlElement( getSequencesAsXml() );
}
