// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemies/RangedEnemy.h"
#include "GOAPAIController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GOAPAILOG,Display, All);

struct FGoapNames
{
	const FName HasAmmo      = FName("HasAmmo");
	const FName LowHealth    = FName("LowHealth");
	const FName InAttackRange = FName("InAttackRange");
	const FName PlayerDead   = FName("PlayerDead");

};
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
	
	TArray<FName> Plan;
	//Each Action In goapactions need to have its corresponding void state.
	void SwitchState(int8 CaseNum);
	
	void PickupAmmo();
	void PickupHealth();
	void MovetoTarget();
	// attack player action is just for a final state. 
	
};
