// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/GoapSystem/GOAPAIController.h"

#include "PinballCharacter.h"
#include "Enemies/RangedEnemy.h"
#include "Enemies/GoapSystem/GOAPActions.h"
#include "Enemies/GoapSystem/GOAPPlanner.h"
#include "Enemies/GoapSystem/GOAPTypes.h"

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
	CurrentWorldState.WorldFacts.Add(FName("HasAmmo"),bHasAmmo);
	CurrentWorldState.WorldFacts.Add(FName("InAttackRange"),bIsInRange);
	CurrentWorldState.WorldFacts.Add(FName("LowHealth"),bLowHealth);
	CurrentWorldState.WorldFacts.Add(FName("PlayerDead"),false);

	FGOAPWorldState GoalWorldState;
	GoalWorldState.WorldFacts.Add(FName("PlayerDead"),true);
	
	//=== one problem is using fnames is the possibility of typos breaking the entire process
	
	TArray<FGOAPAction> PossibleActions;
	
	FGOAPAction PickupAmmo;
	PickupAmmo.ActionName = FName("PickupAmmo");
	PickupAmmo.Cost = 2.0f;
	PickupAmmo.Preconditions.Add(FName("HasAmmo"), false);
	PickupAmmo.Effects.Add(FName("HasAmmo"), true);
	PossibleActions.Add(PickupAmmo);
	
	FGOAPAction PickupHealth;
	PickupHealth.ActionName = FName("PickupHealth");
	PickupHealth.Cost = 2.0f;
	PickupHealth.Preconditions.Add(FName("LowHealth"), true);
	PickupHealth.Effects.Add(FName("LowHealth"), false);
	PossibleActions.Add(PickupHealth);
	
	FGOAPAction MoveToTarget;
	MoveToTarget.ActionName = FName("MoveToTarget");
	MoveToTarget.Cost = 3.0f;
	MoveToTarget.Preconditions.Add(FName("HasAmmo"), true);
	MoveToTarget.Preconditions.Add(FName("LowHealth"), false);
	MoveToTarget.Preconditions.Add(FName("InAttackRange"), false);
	MoveToTarget.Effects.Add(FName("InAttackRange"), true);
	PossibleActions.Add(MoveToTarget);
	
	FGOAPAction AttackPlayer;
	AttackPlayer.ActionName = FName("AttackPlayer");
	AttackPlayer.Cost = 4.0f;
	AttackPlayer.Preconditions.Add(FName("HasAmmo"), true);
	AttackPlayer.Preconditions.Add(FName("InAttackRange"), true);
	AttackPlayer.Effects.Add(FName("PlayerDead"), true);
	PossibleActions.Add(AttackPlayer);

    // === RUN THE PLANNER ===
	UGOAPPlanner* Planner = NewObject<UGOAPPlanner>();
    if (bool bDidMakePlan = Planner->Plan(CurrentWorldState,GoalWorldState,PossibleActions,Plan))
    {
	    UE_LOG(LogTemp, Log, TEXT(" GOAP Plan Found"));
    	for (int32 i = 0; i < Plan.Num(); i++)
    	{
    		UE_LOG(GOAPAILOG, Warning, TEXT("  Step %d: %s"), i + 1, *Plan[i].ToString());
    	}
    }
	else
	{
		UE_LOG(GOAPAILOG, Warning, TEXT("bDidMakePlan: %s"), bDidMakePlan ? TEXT("true") : TEXT("false"));
	}
	
	SwitchState(0);
}

void AGOAPAIController::SwitchState(int8 CaseNum)
{
	
}

void AGOAPAIController::PickupAmmo()
{
	
}

void AGOAPAIController::PickupHealth()
{
	
}

void AGOAPAIController::MovetoTarget()
{
	
}


