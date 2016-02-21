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


typedef Array<int> SliceArray; //the active slices in a step
typedef std::map<int, SliceArray> StepMap; //the step and its active slices
typedef std::map<int, StepMap> SequenceMap; //the sequence and its steps


//ChaseManager gives access to the sequences, their steps and the active slices in the step
//it uses nested maps to provide access and take care of default

class ChaseManager
{
public:
	ChaseManager();
	~ChaseManager();
	
	void setStep ( int sequence, int step, SliceArray activeSlices );
	SliceArray getStep ( int sequence, int step );
	
	void clearAll();
	
	void setName( String name );
	String getName ();
	
	int currentStep;
	int currentSequence;
	
private:
	String chaserName;
	SequenceMap sequenceMap;
	
	
};



#endif  // CHASEMANAGER_H_INCLUDED
