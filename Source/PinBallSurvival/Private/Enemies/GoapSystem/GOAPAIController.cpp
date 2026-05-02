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
		UE_LOG(GOAPAILOG, Log, TEXT(" GOAP AI On Owning Pawn"));

	}
	
	StartAIAction();
}


void AGOAPAIController::SaveAIGOAPData(FGOAPData PassedData)
{
	OwningAIState.bIsLowHealth = PassedData.bIsLowHealth;
	OwningAIState.bHasAmmo = PassedData.bHasAmmo;
	OwningAIState.bIsWithinDistance = PassedData.bIsWithinDistance;
	//.bIsWithinDistance is redundant, need to check this per plan Instance
	UE_LOG(GOAPAILOG, Log, TEXT(" GOAP Saved AI data"));
	
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
	
	UE_LOG(GOAPAILOG, Log, TEXT(" GOAP Making Plan"));

	if (OwningPawn == nullptr) return;
	FGOAPWorldState CurrentWorldState;
	
	float Distance = OwningPawn->GetDistanceTo(PlayerCharacterReference);
	bool bIsInRange = Distance < OwningPawn->GetRange();
	
	UE_LOG(GOAPAILOG, Warning, TEXT("bHasAmmo: %s"), OwningAIState.bHasAmmo ? TEXT("true") : TEXT("false"));
	UE_LOG(GOAPAILOG, Warning, TEXT("bIsInRange: %s"), bIsInRange ? TEXT("true") : TEXT("false"));
	UE_LOG(GOAPAILOG, Warning, TEXT("bLowHealth: %s"), OwningAIState.bIsLowHealth ? TEXT("true") : TEXT("false"));

	CurrentWorldState.WorldFacts.Add(EGOAPFact::HasAmmo,OwningAIState.bHasAmmo);
	CurrentWorldState.WorldFacts.Add(EGOAPFact::InAttackRange,OwningAIState.bIsWithinDistance);
	CurrentWorldState.WorldFacts.Add(EGOAPFact::LowHealth,bIsInRange);
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
	    UE_LOG(GOAPAILOG, Log, TEXT(" GOAP Plan Found"));
    	for (int32 i = 0; i < Plan.Num(); i++)
    	{
    		UE_LOG(GOAPAILOG, Warning, TEXT("  Step %d: %s"), i + 1, *UEnum::GetValueAsString(Plan[i]));
    	}
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
	
	// Grab the first action and remove it from the plan
	EGOAPActionType CurrentAction = Plan[0];
	Plan.RemoveAt(0);
	GEngine->AddOnScreenDebugMessage(-1,10.0f, FColor::Yellow, TEXT("Switching plan"));

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
		WalkTargetLocation = AmmoObject->GetActorLocation();
		MoveToLocation(AmmoObject->GetActorLocation());
		GetWorld()->GetTimerManager().SetTimer(
			CheckIfAtLocation_TimerHandle,
			this,
			&AGOAPAIController::CheckIfAtStoredLocation,
			CheckRate,
			true);
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
		WalkTargetLocation = HealthObject->GetActorLocation();
		MoveToLocation(HealthObject->GetActorLocation());
		GetWorld()->GetTimerManager().SetTimer(
			CheckIfAtLocation_TimerHandle,
			this,
			&AGOAPAIController::CheckIfAtStoredLocation,
			CheckRate,
			true);
	}
}

void AGOAPAIController::MovetoTarget()
{
	GetWorld()->GetTimerManager().SetTimer(
		WalkTimer_TimeHandler,
		this,
		&AGOAPAIController::Timer_MoveToTarget,
		WalkRate,
		true);
}

void AGOAPAIController::Timer_MoveToTarget()
{
	float DistanceToPlayer = OwningPawn->GetDistanceTo(PlayerCharacterReference);
	if (DistanceToPlayer <= OwningPawn->GetRange())
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f, FColor::Yellow, TEXT("Move to target, distance reached"));
		SwitchStateOnPlan();
		GetWorld()->GetTimerManager().ClearTimer(WalkTimer_TimeHandler);
		return;
	}
	const FVector PlayerLocation = PlayerCharacterReference->GetActorLocation();
	const float DesiredDistance = OwningPawn->GetRange() - 100.0f;
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

		MakePlan();
		GetWorld()->GetTimerManager().ClearTimer(AttackPlayer_TimerHandle);
		return;
	}
	OwningPawn->ShootWeapon();
	GEngine->AddOnScreenDebugMessage(-1,10.0f, FColor::Green, TEXT("Shooting Player"));
	
}

void AGOAPAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	/*if (Result.IsSuccess())
	{
		// Arrived at destination — execute next action in GOAP plan
		SwitchStateOnPlan();
	}
	else
	{
		// Movement failed — replan
		MakePlan();
	}*/
}

void AGOAPAIController::CheckIfAtStoredLocation()
{
	const float DistSq = FVector::DistSquared(OwningPawn->GetActorLocation(), WalkTargetLocation);
	if (DistSq <= FMath::Square(ArrivalTolerance))
	{
		// Stop the timer so it doesn't keep firing
		SwitchStateOnPlan();
		GEngine->AddOnScreenDebugMessage(-1,10.0f, FColor::Yellow, TEXT("CheckIfAtStoredLocation done"));
		GetWorld()->GetTimerManager().ClearTimer(CheckIfAtLocation_TimerHandle);
	}
}

 
