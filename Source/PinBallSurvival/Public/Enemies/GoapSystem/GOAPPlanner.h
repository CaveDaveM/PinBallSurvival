// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/GoapSystem/GOAPTypes.h"
#include "UObject/Object.h"
#include "GOAPPlanner.generated.h"

/**
 * The A* algorithm tracks the state and the cost of each step
 * to do this i need a node, and each node 
 */

struct FGOAPAction;

USTRUCT()
struct FGOAPNode
{
	GENERATED_BODY()
	
	FGOAPWorldState WorldState;
	
	FName ActionName;
	
	float GCost = 0.0f;
	float HCost = 0.0f;
	float FCost() const { return GCost + HCost; }
	
	int32 ParentIndex = -1;
};
UCLASS()
class PINBALLSURVIVAL_API UGOAPPlanner : public UObject
{
	GENERATED_BODY()
public:
	bool Plan(
		const FGOAPWorldState& CurrentState,
		const FGOAPWorldState& GOAlState,
		const TArray<FGOAPAction>& AvailableActions,
		TArray<FName>& OutPlan);
	
private:
	void ReconstructPlan(
		const TArray<FGOAPNode>& ClosedList,
		int32 GoalIndex,
		TArray<FName>& OutPlan);
};
