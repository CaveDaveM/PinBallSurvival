// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPawn.h"
#include "Enemies/EnemyBaseClass.h"
#include "GoapSystem/GOAPAIController.h"
#include "Interfaces/WeaponInterface.h"
#include "RangedEnemy.generated.h"

class UFloatingPawnMovement;
class AEnemyProjectiles;
class APinballCharacter;
class UStaticMeshComponent;



DECLARE_MULTICAST_DELEGATE_OneParam(FOnGOAPData,FGOAPData);

UCLASS()
class PINBALLSURVIVAL_API ARangedEnemy : public AMyPawn, public IWeaponInterface
{
	GENERATED_BODY()
	
public:
	ARangedEnemy();
	
	FOnGOAPData OnGOAPData;
	//Getters and setters, if needed
	int GetAmmo() const {return CurrentAmmo;}
	float GetRange() const {return TargetDistance;}
	float GetHealth() const {return Health;}
	
	//Action Functions
	void ShootWeapon();
	virtual void ApplyDamage(float DamageAmount) override;
	
protected:
	virtual void AddAmmo(const int32 Ammo) override;
	virtual void BeginPlay() override;
	void CollectGOAPData();
	
	int32 CurrentAmmo = 12;
	int32 MaxAmmo = 12;
	int32 LowAmmoThreshold = 5; 
	float TargetDistance = 500.0f;
	
	FGOAPData GOAPData;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyProjectiles> ProjectileClass;
	FTimerHandle ShootTimer_TimeHandler; 
private:
	UPROPERTY()
	APawn* PlayerCharacterReference;
	
	
};
