// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/GoapSystem/GOAPAIController.h"
#include "PinballCharacter.h"
#include "Enemies/GoapSystem/GOAPActions.h"
#include "Enemies/GoapSystem/GOAPPlanner.h"
#include "Enemies/GoapSystem/GOAPTypes.h"
#include "Enemies/RangedEnemy.h"

DEFINE_LOG_CATEGORY(GOAPAILOG);

AGOAPAIController::AGOAPAIController()
{
}

void AGOAPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	OwningPawn = Cast<ARangedEnemy>(InPawn);
	PlayerCharacterReference = Cast<APinballCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	StartAIAction();
}


void AGOAPAIController::StartAIAction()
{
	MakePlan();
}

void AGOAPAIController::MakePlan()
{
	if (OwningPawn == nullptr) return;
	FGOAPWorldState CurrentWorldState;
	
	bool bHasAmmo =	OwningPawn->GetAmmo() > 2; 
	float Distance = OwningPawn->GetDistanceTo(PlayerCharacterReference);
	bool bIsInRange = Distance < OwningPawn->GetRange();
	bool bLowHealth = OwningPawn->GetHealth() < 20.0f; 
	UE_LOG(GOAPAILOG, Warning, TEXT("bHasAmmo: %s"), bHasAmmo ? TEXT("true") : TEXT("false"));
	UE_LOG(GOAPAILOG, Warning, TEXT("bIsInRange: %s"), bIsInRange ? TEXT("true") : TEXT("false"));
	UE_LOG(GOAPAILOG, Warning, TEXT("bLowHealth: %s"), bLowHealth ? TEXT("true") : TEXT("false"));
	CurrentWorldState.WorldFacts.Add(EGOAPFact::HasAmmo,bHasAmmo);
	CurrentWorldState.WorldFacts.Add(EGOAPFact::InAttackRange,bIsInRange);
	CurrentWorldState.WorldFacts.Add(EGOAPFact::LowHealth,bLowHealth);
	CurrentWorldState.WorldFacts.Add(EGOAPFact::PlayerDead,false);

	FGOAPWorldState GoalWorldState;
	GoalWorldState.WorldFacts.Add(EGOAPFact::PlayerDead,true);
	
	//=== one problem is using fnames is the possibility of typos breaking the entire process
	
	TArray<FGOAPAction> PossibleActions;
	
	FGOAPAction PickupAmmo;
	PickupAmmo.ActionType = EGOAPActionType::PickupAmmo;
	PickupAmmo.Cost = 2.0f;
	PickupAmmo.Preconditions.Add(EGOAPFact::HasAmmo, false);
	PickupAmmo.Effects.Add(EGOAPFact::HasAmmo, true);
	PossibleActions.Add(PickupAmmo);
	
	FGOAPAction PickupHealth;
	PickupHealth.ActionType = EGOAPActionType::PickupHealth;
	PickupHealth.Cost = 2.0f;
	PickupHealth.Preconditions.Add(EGOAPFact::LowHealth, true);
	PickupHealth.Effects.Add(EGOAPFact::LowHealth, false);
	PossibleActions.Add(PickupHealth);
	
	FGOAPAction MoveToTarget;
	MoveToTarget.ActionType = EGOAPActionType::MoveToTarget;
	MoveToTarget.Cost = 3.0f;
	MoveToTarget.Preconditions.Add(EGOAPFact::HasAmmo, true);
	MoveToTarget.Preconditions.Add(EGOAPFact::LowHealth, false);
	MoveToTarget.Preconditions.Add(EGOAPFact::InAttackRange, false);
	MoveToTarget.Effects.Add(EGOAPFact::InAttackRange, true);
	PossibleActions.Add(MoveToTarget);
	
	FGOAPAction AttackPlayer;
	AttackPlayer.ActionType = EGOAPActionType::AttackPlayer;
	AttackPlayer.Cost = 4.0f;
	AttackPlayer.Preconditions.Add(EGOAPFact::HasAmmo, true);
	AttackPlayer.Preconditions.Add(EGOAPFact::InAttackRange, true);
	AttackPlayer.Effects.Add(EGOAPFact::PlayerDead, true);
	PossibleActions.Add(AttackPlayer);

    // === RUN THE PLANNER ===
	UGOAPPlanner* Planner = NewObject<UGOAPPlanner>();
    if (bool bDidMakePlan = Planner->Plan(CurrentWorldState,GoalWorldState,PossibleActions,Plan))
    {
	    UE_LOG(LogTemp, Log, TEXT(" GOAP Plan Found"));
    }
	else
	{
		UE_LOG(GOAPAILOG, Warning, TEXT("bDidMakePlan: %s"), bDidMakePlan ? TEXT("true") : TEXT("false"));
	}
	SwitchStateOnPlan(0);
}

void AGOAPAIController::SwitchStateOnPlan(int32 CurrentIndex)
{
	switch (Plan[CurrentIndex]) {
	case EGOAPActionType::PickupAmmo:
		{
			
			break;
		}
	case EGOAPActionType::PickupHealth:
		{
			
			break;
		}
	case EGOAPActionType::MoveToTarget:
		{
			
			break;
		}
	case EGOAPActionType::AttackPlayer:
		{
			
			break;
		}
	}

}

void AGOAPAIController::PickupAmmo(int32 CurrentIndex)
{
	
}

void AGOAPAIController::PickupHealth(int32 CurrentIndex)
{
	
}

void AGOAPAIController::MovetoTarget(int32 CurrentIndex)
{
	
}


 