// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPawn.h"
#include "BasicEnemy.generated.h"

class APinballCharacter;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API ABasicEnemy : public AMyPawn
{
	GENERATED_BODY()
public:
	ABasicEnemy();

protected:
	
	UPROPERTY()
	APinballCharacter* PlayerCharacter;
	
	UPROPERTY(EditAnywhere)
	float EnemyDamage = 5;
	
	FTimerHandle HitPlayer_TimerHandle;
	
	virtual void BeginPlay() override;
	void ApplyDamageIfInRange();
};
