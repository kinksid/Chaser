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
	
	int skipToNextSequence();
	int skipToPreviousSequence();
	
	void skipToSequence ( int i );
	void skipToStep ( int i );
	
	//will return the new current step
	int skipToNextStep();
	int skipToPreviousStep();
	
	//will return the new max amount of steps in the current sequence
	int addStep();
	int removeStep();
	
	int getLastStep();
	
	int getCurrentStep();
	int getCurrentSequence();
	
	
	
private:
	
	void fillSequence();
	int currentStep;
	int currentSequence;
	String chaserName;
	SequenceMap sequenceMap;
	StepMap stepMap;
	
};



#endif  // CHASEMANAGER_H_INCLUDED
