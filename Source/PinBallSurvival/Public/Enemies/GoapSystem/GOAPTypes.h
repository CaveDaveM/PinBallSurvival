// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GOAPTypes.generated.h"

/** Comments in the goap system will be both comments about what the code is doing and my overall thought process
 *	Explanations about the code will be marked with a //
    my thought process will be marked with the double asterix style of commenting*/

UENUM()
enum class EGOAPFact : uint8
{
	HasAmmo,
	LowHealth,
	InAttackRange,
	PlayerDead,
};

UENUM()
enum class EGOAPActionType : uint8
{
	PickupAmmo,
	PickupHealth,
	MoveToTarget,
	AttackPlayer
};

USTRUCT()
struct FGOAPWorldState
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<EGOAPFact, bool> WorldFacts;
	
	bool SatisfiesGoals(const FGOAPWorldState& Goal)
	{
		for (const TPair<EGOAPFact, bool>& GoalFacts : Goal.WorldFacts)
		{
			//making the currentvaule a pointer is necessary to check if the key actaully exists
			bool *CurrentValue = WorldFacts.Find(GoalFacts.Key);
			if (CurrentValue == nullptr || *CurrentValue != GoalFacts.Value)
			{
				return false;
			}
		}
		return true;// only retruns true if every single goal was true
	} 
	
	FGOAPWorldState ApplyEffects(const TMap<EGOAPFact, bool>& Effects)
	{
		FGOAPWorldState NewState = *this;
		for (const TPair<EGOAPFact, bool>& Effect : Effects)
		{
			NewState.WorldFacts.Add(Effect.Key, Effect.Value);
		}
		return NewState;
	}
	//heuristic is the cost of how many steps remains to the goal
	int32 HeuristicCost(const FGOAPWorldState& Goal) const
	{
		int32 HeuristicValue = 0;
		for (const TPair<EGOAPFact, bool>& GoalFacts : Goal.WorldFacts)
		{
			const bool* CurrentValue = WorldFacts.Find(GoalFacts.Key);
			if (CurrentValue == nullptr || *CurrentValue != GoalFacts.Value)
			{
				HeuristicValue++;
			}
		}
		return HeuristicValue;
	}
	
};