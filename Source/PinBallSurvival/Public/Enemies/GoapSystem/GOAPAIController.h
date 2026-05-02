// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GOAPAIController.generated.h"


class APinballCharacter;
class ARangedEnemy;
class UNavigationSystemV1;
class UWorldStateSubsystem;
enum class EGOAPActionType : uint8;
DECLARE_LOG_CATEGORY_EXTERN(GOAPAILOG, Display, All);

USTRUCT()
struct FGOAPData
{
	GENERATED_BODY()
	
	bool bHasAmmo;
	bool bIsLowHealth;
	bool bIsWithinDistance;
	
};

UCLASS()
class PINBALLSURVIVAL_API AGOAPAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGOAPAIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	void StartAIAction();
	void MakePlan();
	
	UPROPERTY()
	ARangedEnemy* OwningPawn;
	UPROPERTY()
	APinballCharacter* PlayerCharacterReference;
	UPROPERTY()
	UWorldStateSubsystem* WorldState;
	UPROPERTY(EditAnywhere, Category = "AI")
	UNavigationSystemV1* NavSystem;

	//Plan to be performed
	TArray<EGOAPActionType> Plan;
	
	//AttackingData
	FTimerHandle AttackPlayer_TimerHandle;
	float ShootRate = 3.0f;
	
	FTimerHandle WalkTimer_TimeHandler;
	float WalkRate = 0.2f;
	
	FTimerHandle CheckIfAtLocation_TimerHandle;
	float CheckRate = 0.5;
	float ArrivalTolerance = 50.0f;
	FVector WalkTargetLocation;
	void CheckIfAtStoredLocation();
	
	//Data required for the GOAP Plan
	FGOAPData OwningAIState;
	void SaveAIGOAPData(FGOAPData PassedData);
	
	//Each Action In goapactions need to have its corresponding void state.
	void SwitchStateOnPlan();
	void PickupAmmo();
	void PickupHealth();
	void MovetoTarget();
	UFUNCTION()
	void Timer_MoveToTarget();
	void AttackPlayer();
	void ShootPlayer();

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	// attack player action is just for a final state. 
	
};
