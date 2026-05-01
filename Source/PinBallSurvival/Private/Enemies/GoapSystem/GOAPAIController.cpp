// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/GoapSystem/GOAPAIController.h"

#include "NavigationSystem.h"
#include "PinballCharacter.h"
#include "Enemies/GoapSystem/GOAPActions.h"
#include "Enemies/GoapSystem/GOAPPlanner.h"
#include "Enemies/GoapSystem/GOAPTypes.h"
#include "Enemies/RangedEnemy.h"
#include "Navigation/PathFollowingComponent.h"
#include "WorldObjects/BaseWorldObject.h"
#include "WorldObjects/Managers/WorldObjectData.h"

DEFINE_LOG_CATEGORY(GOAPAILOG);

AGOAPAIController::AGOAPAIController()
{
}

void AGOAPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OwningPawn = Cast<ARangedEnemy>(InPawn);
	if (OwningPawn)
	{
		OwningPawn->OnGOAPData.AddUObject(this,&AGOAPAIController::SaveAIGOAPData);
	}
	
	StartAIAction();
}

void AGOAPAIController::SaveAIGOAPData(FGOAPData PassedData)
{
	OwningAIState.bIsLowHealth = PassedData.bIsLowHealth;
	OwningAIState.bHasAmmo = PassedData.bHasAmmo;
	OwningAIState.bIsWithinDistance = PassedData.bIsWithinDistance;
	
	MakePlan();
}

void AGOAPAIController::StartAIAction()
{
	PlayerCharacterReference = Cast<APinballCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	WorldState = GetWorld()->GetSubsystem<UWorldStateSubsystem>();
	NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
}

void AGOAPAIController::MakePlan()
{
	if (OwningPawn == nullptr) return;
	FGOAPWorldState CurrentWorldState;
	
	CurrentWorldState.WorldFacts.Add(EGOAPFact::HasAmmo,OwningAIState.bHasAmmo);
	CurrentWorldState.WorldFacts.Add(EGOAPFact::InAttackRange,OwningAIState.bIsWithinDistance);
	CurrentWorldState.WorldFacts.Add(EGOAPFact::LowHealth,OwningAIState.bIsLowHealth);
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
	SwitchStateOnPlan();
}


void AGOAPAIController::SwitchStateOnPlan()
{

	if (Plan.Num() == 0)
	{
		MakePlan();
	}
	
	GEngine->AddOnScreenDebugMessage(-1,10.0f, FColor::Yellow, TEXT("Switching State"));

	// Grab the first action and remove it from the plan
	EGOAPActionType CurrentAction = Plan[0];
	Plan.RemoveAt(0);

	switch (CurrentAction)
	{
	case EGOAPActionType::PickupAmmo:
		{
			PickupAmmo();
			break;
		}
	case EGOAPActionType::PickupHealth:
		{
			PickupHealth();
			break;
		}
	case EGOAPActionType::MoveToTarget:
		{
			MovetoTarget();
			break;
		}
	case EGOAPActionType::AttackPlayer:
		{
			UE_LOG(LogNavigation, Log, TEXT("Attacking Player"));
			AttackPlayer();
			break;
		}
	}
}

void AGOAPAIController::PickupAmmo()
{
	ABaseWorldObject* AmmoObject = WorldState->GetClosestWorldObjectByType(
		EObjectType::AmmoObject,
		OwningPawn->GetActorLocation());
	
	if (!AmmoObject)
	{
		//if there are no present health objects in the world, disregard and try to kill the player anyway
		SwitchStateOnPlan();
	}
	else
	{
		MoveToLocation(AmmoObject->GetActorLocation());
	}
	
}

void AGOAPAIController::PickupHealth()
{
	ABaseWorldObject* HealthObject = WorldState->GetClosestWorldObjectByType(
		EObjectType::HealingObject,
		OwningPawn->GetActorLocation());
	
	if (!HealthObject)
	{
		//if there are no present health objects in the world, disregard and try to kill the player anyway
		SwitchStateOnPlan();
	}
	else
	{
		MoveToLocation(HealthObject->GetActorLocation());
	}
}

void AGOAPAIController::MovetoTarget()
{
	if (!PlayerCharacterReference || !OwningPawn)
	{
		return;
	}

	const FVector PlayerLocation = PlayerCharacterReference->GetActorLocation();

	const float DesiredDistance = OwningPawn->GetRange();

	// direction from player to ai
	const FVector DirectionToAI = (OwningPawn->GetActorLocation() - PlayerCharacterReference->GetActorLocation()).GetSafeNormal();

	// Point on the circumference closest to the AI's current position
	FVector TargetPoint;
	TargetPoint.X = PlayerLocation.X + DirectionToAI.X * DesiredDistance;
	TargetPoint.Y = PlayerLocation.Y + DirectionToAI.Y * DesiredDistance;
	TargetPoint.Z = PlayerLocation.Z;
	
	FNavLocation TargetLocation;

	if (NavSystem && NavSystem->ProjectPointToNavigation(TargetPoint, TargetLocation))
	{
		MoveToLocation(TargetLocation.Location, 50.0f);
	}
}

void AGOAPAIController::AttackPlayer()
{
	GetWorld()->GetTimerManager().SetTimer(
		AttackPlayer_TimerHandle,
		this,
		&AGOAPAIController::ShootPlayer,
		ShootRate,
		true);
}

void AGOAPAIController::ShootPlayer()
{
	float DistanceToPlayer = OwningPawn->GetDistanceTo(PlayerCharacterReference);
	if (DistanceToPlayer > OwningPawn->GetRange())
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackPlayer_TimerHandle);
		MakePlan();
		return;
	}
	OwningPawn->ShootWeapon();
	
}

void AGOAPAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	if (Result.IsSuccess())
	{
		// Arrived at destination — execute next action in GOAP plan
		SwitchStateOnPlan();
	}
	else
	{
		// Movement failed — replan
		MakePlan();
	}
}


 