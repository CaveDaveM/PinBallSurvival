// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemies/RangedEnemy.h"
#include "GOAPAIController.generated.h"


enum class EGOAPActionType : uint8;
DECLARE_LOG_CATEGORY_EXTERN(GOAPAILOG, Display, All);


/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API AGOAPAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGOAPAIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	void MovePawn(FVector NewLocation);
	void StartAIAction();
	void MakePlan();
	
	UPROPERTY()
	ARangedEnemy* OwningPawn;
	UPROPERTY()
	APinballCharacter* PlayerCharacterReference;
	
	TArray<EGOAPActionType> Plan;
	//Each Action In goapactions need to have its corresponding void state.
	void SwitchStateOnPlan(int32 CurrentIndex);
	
	void PickupAmmo(int32 CurrentIndex);
	void PickupHealth(int32 CurrentIndex);
	void MovetoTarget(int32 CurrentIndex);
	// attack player action is just for a final state. 
	
};
