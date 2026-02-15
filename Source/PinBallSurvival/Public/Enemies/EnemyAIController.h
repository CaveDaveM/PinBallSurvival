// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/** this enemy class is will be only for simple enemy AI
 * only walks towards the player and attacks with close combat
 */

UCLASS()
class PINBALLSURVIVAL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	FVector FindPlayerLocation();
	
	UFUNCTION()
	void MoveEnemyToPlayer();
	
	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed = 800.0f;
	
	UPROPERTY()
	APawn* PlayerCharacterReference; 
	
	FTimerHandle Action_TimeHandler;
};
