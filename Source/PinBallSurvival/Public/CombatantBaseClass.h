// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HealthInterface.h"
#include "Interfaces/WeaponInterface.h"
#include "CombatantBaseClass.generated.h"

class UInputAction;
class ABasicProjectile;
class USphereComponent;

UCLASS()
class PINBALLSURVIVAL_API ACombatantBaseClass : public APawn, public IHealthInterface, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatantBaseClass();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> ClickAction;
	
	UPROPERTY()
	APlayerController* PC;

	
	virtual void BeginPlay() override;
	
	void OnClick();
	void FireWeapon(FVector ClickedLocation);
	void UpdatePlayerHealth();
	virtual void UpdateHudStats();
	virtual void ShotFired();

public:
	virtual void AddAmmo(int32 Ammo) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABasicProjectile> ProjectileWeapons;
	
	UPROPERTY()
	TArray<AActor*> ProximityEnemyArray;
	FTimerHandle FireWeapon_TimerHandle;
	float BaseDamage = 40;
	float TotalDamage = 0;
	
	int32 MaxAmmo = 120;
	int32 CurrentAmmo = 120;
};
