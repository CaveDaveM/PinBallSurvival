// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBaseClass.h"
#include "Interfaces/WeaponInterface.h"
#include "RangedEnemy.generated.h"

class AEnemyProjectiles;
class APinballCharacter;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API ARangedEnemy : public AEnemyBaseClass, public IWeaponInterface
{
	GENERATED_BODY()
	
public:
	ARangedEnemy();
	void GetAmmo();
protected:
	virtual void BeginPlay() override;
	void ShootWeapon();
	
	int32 Ammo = 12;
	float TargetDistance = 500.0f;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyProjectiles> ProjectileClass;
	FTimerHandle ShootTimer_TimeHandler;
private:
	UPROPERTY()
	APawn* PlayerCharacterReference;
	
	
};
