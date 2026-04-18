// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPTypes.h"
#include "GOAPActions.Generated.h"

USTRUCT()
struct FGOAPAction
{
	GENERATED_BODY()	
	//name of the planned action
	UPROPERTY(EditAnywhere)
	FName ActionName;
	//the cost that the action will have, from what i currently understand this is then used
	//to create a plan of the lowest costing parts 
	UPROPERTY(EditAnywhere)
	float Cost = 1.0f;
	// another important thing to consider is the precondition of each action
	//so in my case, if does the enemy have a charge to destroy a health pack
	TMap<FName, bool> Preconditions;
	//Effects are the result of each action so if you take course of action a, what changes
	TMap<FName, bool> Effects;
	
	bool ArePreconditionsMet(const FGOAPWorldState& State) const
	{
		for (const TPair<FName, bool> Precondition :Preconditions)
		{
			const bool* Value = State.WorldFacts.Find(Precondition.Key);
			if (Value == nullptr || *Value != Precondition.Value)
			{
				return false;
			}
		}
		return true;
	}
};
