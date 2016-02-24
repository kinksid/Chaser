/*
  ==============================================================================

    ChaseManager.h
    Created: 21 Feb 2016 5:04:36pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef CHASEMANAGER_H_INCLUDED
#define CHASEMANAGER_H_INCLUDED

#include <map>
#include "Slice.h"

typedef Array<int> SliceIndexArray; //the active slices in a step
typedef std::map<int, SliceIndexArray> StepMap; //the step and its active slices
typedef std::map<int, StepMap> SequenceMap; //the sequence and its steps
typedef std::map<int, String> NameMap;


//ChaseManager gives access to the sequences, their steps and the active slices in the step
//it uses nested maps to provide access and take care of default

class ChaseManager
{
public:
	ChaseManager();
	~ChaseManager();
	
	void setStep ( int sequence, int step, SliceIndexArray activeSlices );
	void setStep ( SliceIndexArray activeSlices );
	SliceIndexArray getStep ( int sequence, int step );
	SliceIndexArray getStep ();
	
	void clearAll();
	
	void setName( String name );
	String getName ();
	
	void skipToSequence ( int i );
	void skipToStep ( int i );
	
	//will return the new current step
	int skipToNextStep();
	int skipToPreviousStep();
	
	//will return the new current sequence
	int skipToNextSequence();
	int skipToPreviousSequence();
	
	//will return the new max amount of steps in the current sequence
	//steps are always added and removed from the end of the sequence
	int addStep();
	int removeStep();
	
	//will return the new max amount of sequences
	int addSequence( );
	int removeCurrentSequence( );
	
	int getLastStep();
	int getLastSequence();
	
	int getCurrentStep();
	int getCurrentSequence();
	
	String getCurrentSequenceName();
	void setCurrentSequenceName( String newName );
	
	
	
private:
	
	void fillSequence();
	int currentStep;
	int currentSequence;
	String chaserName;
	SequenceMap sequenceMap;
	StepMap stepMap;
	NameMap nameMap;
	
};



#endif  // CHASEMANAGER_H_INCLUDED
