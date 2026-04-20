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
	int GetAmmo() const {return CurrentAmmo;}
	float GetRange() const {return TargetDistance;}
	float GetHealth() const {return Health;}
	
protected:
	virtual void AddAmmo(const int32 Ammo) override;
	virtual void BeginPlay() override;
	void ShootWeapon();
	
	float Health = 15.0f;
	int32 CurrentAmmo = 12;
	int32 MaxAmmo = 12;
	float TargetDistance = 500.0f;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyProjectiles> ProjectileClass;
	FTimerHandle ShootTimer_TimeHandler;
private:
	UPROPERTY()
	APawn* PlayerCharacterReference;
	
	
};
