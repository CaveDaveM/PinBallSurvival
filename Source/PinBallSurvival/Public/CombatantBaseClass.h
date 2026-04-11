// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HealthInterface.h"
#include "CombatantBaseClass.generated.h"

class ABasicProjectile;
class USphereComponent;

UCLASS()
class PINBALLSURVIVAL_API ACombatantBaseClass : public APawn, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatantBaseClass();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* PawnDetectionSphere;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* 
		OtherComp, 
		int32 OtherBodyIndex);
	
	void FireWeapon();
	void UpdatePlayerHealth();
	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABasicProjectile> ProjectileWeapons;

	UPROPERTY()
	TArray<AActor*> ProximityEnemyArray;
	
	FTimerHandle FireWeapon_TimerHandle;
	
	UPROPERTY()
	float BaseDamage = 40;
	
	UPROPERTY()
	float TotalDamage = 0;
};
