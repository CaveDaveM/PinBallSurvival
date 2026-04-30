// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/GoapSystem/GOAPPlanner.h"

#include "Enemies/GoapSystem/GOAPActions.h"

bool UGOAPPlanner::Plan(const FGOAPWorldState& CurrentState, const FGOAPWorldState& GOAlState,
                        const TArray<FGOAPAction>& AvailableActions, TArray<EGOAPActionType>& OutPlan)
{
	//Since we are passing the outplan as a reference, we need to clear this list 
	OutPlan.Empty();
	
	//the open list will store nodes that arnt explored yet
	TArray<FGOAPNode> OpenList;
	//the closed list for nodes that have been explored and expanded
	TArray<FGOAPNode> ClosedList;
	
	
	FGOAPNode StartingNode;
	StartingNode.WorldState = CurrentState;
	StartingNode.GCost = 0.0f;
	StartingNode.HCost = CurrentState.HeuristicCost(GOAlState);
	StartingNode.ParentIndex = -1; //arrays in C start at -1 so this is actually the firstindex
	StartingNode.bIsStartingNode = true;
	
	OpenList.Add(StartingNode);
	//TODO: figgure out the perfect amount for max iterations.
	int32 MaxIterations = 1000;
	
	while (OpenList.Num() > 0 && MaxIterations-- > 0)
	{
		int32 BestIndex = 0;
		float BestFCost = OpenList[0].FCost();
		
		for (int32 i = 1; i < OpenList.Num(); i++)
		{
			if (OpenList[i].FCost() < BestFCost)
			{
				BestFCost = OpenList[i].FCost();
				BestIndex = i;
			}
		}
		
		FGOAPNode CurrentNode = OpenList[BestIndex];
		OpenList.RemoveAt(BestIndex);
		int32 CurrentClosedIndex = ClosedList.Add(CurrentNode);
		
		if (CurrentNode.WorldState.SatisfiesGoals(GOAlState))
		{
			ReconstructPlan(ClosedList,CurrentClosedIndex,OutPlan);
			return true;
		}
		
		for (const FGOAPAction& Action : AvailableActions)
		{
			if (!Action.ArePreconditionsMet(CurrentNode.WorldState))
			{
				continue;
			}
			
			FGOAPWorldState NewState = CurrentNode.WorldState.ApplyEffects(Action.Effects);
			
			float NewGCost = CurrentNode.GCost + Action.Cost;
			float NewHCost = NewState.HeuristicCost(NewState);
			
			FGOAPNode NewNode;
			NewNode.WorldState = NewState;
			NewNode.ActionType = Action.ActionType;
			NewNode.GCost = NewGCost;
			NewNode.HCost = NewHCost;
			NewNode.ParentIndex = CurrentClosedIndex;
			NewNode.bIsStartingNode = false;
			
			OpenList.Add(NewNode);
		}
	}
	return false; // this means there is no plan and the goal cant be reached
}

void UGOAPPlanner::ReconstructPlan(const TArray<FGOAPNode>& ClosedList, int32 GoalIndex, TArray<EGOAPActionType>& OutPlan)
{
	int32 CurrentIndex = GoalIndex;
	
	while (CurrentIndex != -1)
	{
		const FGOAPNode& Node = ClosedList[CurrentIndex];
		
		if (!Node.bIsStartingNode)
		{
			OutPlan.Insert(Node.ActionType, 0);
		}
		CurrentIndex = Node.ParentIndex;
	}
	
}
