// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemies/RangedEnemy.h"
#include "GOAPAIController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GOAPAILOG,Display, All);
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
	void MakePlan();
	
	UPROPERTY()
	ARangedEnemy* OwningPawn;
	UPROPERTY()
	APinballCharacter* PlayerCharacterReference;
};
