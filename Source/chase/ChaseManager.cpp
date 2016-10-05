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
	//create 16 sequences with 16 empty steps
	sequences.resize( 16 );
	for ( int i = 0; i < sequences.size(); i++  )
	{
		Sequence sequence;
		sequence.steps.resize( 16 );
		sequence.name = "Sequence " + String( i + 1 );

		sequences.set( i, sequence );		
	}

	currentStep = 0;
	currentSequence = 0;
}

ChaseManager::~ChaseManager()
{

}

void ChaseManager::setStep( int sequenceIndex, int stepIndex, Step activeSlices )
{
	if ( sequences.size() <= sequenceIndex )
	{
		DBG( "Trying to set a sequence which doesn't exist!" );
		return;
	}
	if ( sequences[ sequenceIndex ].steps.size() <= stepIndex )
	{
		DBG( "Trying to set a step which doesn't exist!" );
		return;
	}
	
	Sequence sequence = sequences[ sequenceIndex ];
	sequence.steps.set( stepIndex, activeSlices );
	sequences.set( sequenceIndex, sequence );
	writeToXml();
}

void ChaseManager::setCurrentStep( Step activeSlices )
{
	setStep( getCurrentSequenceIndex(), getCurrentStepIndex(), activeSlices );
}

Step ChaseManager::getStep( int sequence, int step )
{
	if ( sequences.size() > sequence )
		if ( sequences[ sequence ].steps.size() > step )
			return sequences[ sequence ].steps[ step ];

	return Array < int64 > {};
}

Step ChaseManager::getCurrentStep()
{
	return getStep( getCurrentSequenceIndex(), getCurrentStepIndex() );
}

void ChaseManager::clearAll()
{
	sequences.clear();
	writeToXml();
}

void ChaseManager::fillSequence()
{
	//first make sure we have enough sequences
	if ( sequences.size() <= currentSequence )
		sequences.resize( currentSequence + 1 );

	//then make sure the sequence is empty
	//otherwise we can just leave it
	if ( sequences[ currentSequence ].steps.size() == 0 )
	{
		//create 16 steps for this sequence and name it
		Sequence sequence;
		sequence.steps.resize( 16 );
		sequence.name = "Sequence " + String( currentSequence + 1 );
		sequences.set( currentSequence, sequence );
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
	if ( i > getLastStepIndex() )
		i = getLastStepIndex();
	
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
	Sequence sequence = sequences[ getCurrentSequenceIndex() ];
	sequence.steps.resize( sequence.steps.size() + 1 );

	sequences.set( getCurrentSequenceIndex(), sequence );
	writeToXml();

	return getLastStepIndex();
}

int ChaseManager::removeStep()
{
	//only remove as long as we still have more than 1 step
	if ( getLastStepIndex() > 0 )
	{
		Sequence sequence = sequences[ getCurrentSequenceIndex() ];
		sequence.steps.resize( sequence.steps.size() - 1 );

		sequences.set( getCurrentSequenceIndex(), sequence );
		writeToXml();
	}

	return getLastStepIndex();
}

int ChaseManager::getLastStepIndex()
{
	return sequences[ getCurrentSequenceIndex() ].steps.size() - 1;
}

int ChaseManager::getLastSequenceIndex()
{
	return sequences.size() - 1;
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
	if ( currentSequence > getLastSequenceIndex() )
		currentSequence = getLastSequenceIndex();
	
	return currentSequence;
}

String ChaseManager::getCurrentSequenceName()
{
	return sequences[ getCurrentSequenceIndex() ].name;
}

void ChaseManager::setCurrentSequenceName( juce::String newName )
{
	Sequence sequence = sequences[ getCurrentSequenceIndex() ];
	sequence.name = newName;
	
	sequences.set( getCurrentSequenceIndex(), sequence );

	writeToXml();
}

XmlElement* ChaseManager::getSequencesAsXml()
{
	XmlElement* sequencesXml = new XmlElement( "sequences" );
	sequencesXml->setAttribute( "count", sequences.size() );

	//loop through all the sequences
	for ( int i = 0; i < sequences.size(); i++ )
	{
		Sequence sequence = sequences[ i ];
		//check if the sequence has anything to save
		if ( !sequence.isEmpty() || sequence.steps.size() != 16 )
		{
			//for every sequence, create an xmlelement and store the name
			XmlElement* sequenceXml = new XmlElement( "sequence" );
			sequenceXml->setAttribute( "nr", i );
			sequenceXml->setAttribute( "name", sequences[ i ].name );
			sequencesXml->addChildElement( sequenceXml );

			//then create an xmlelement to store the steps
			XmlElement* stepsXml = new XmlElement( "steps" );
			stepsXml->setAttribute( "count", sequence.steps.size() );
			sequenceXml->addChildElement( stepsXml );

			//loop through this sequence's steps
			for ( auto step : sequence.steps )
			{
				//only store filled steps
				if ( step.size() > 0 )
				{
					//for every step, create an xmlelement and store the step nr
					XmlElement* stepXml = new XmlElement( "step" );
					stepXml->setAttribute( "nr", sequence.steps.indexOf( step ) );
					stepsXml->addChildElement( stepXml );

					//loop through this step's active slices
					for ( int64 sliceUid : step )
					{
						//for every active slice, create an xmlelement and store the slice nr
						XmlElement* sliceXml = new XmlElement( "slice" );
						sliceXml->setAttribute( "uniqueId", String( sliceUid ) );
						stepXml->addChildElement( sliceXml );
					}
				}
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

void ChaseManager::setSequences( Array<Sequence> newChaser )
{
	sequences = newChaser;
}


