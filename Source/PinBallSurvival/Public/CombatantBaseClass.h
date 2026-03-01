// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatantBaseClass.generated.h"

class UProjectileWeapons;
class USphereComponent;

UCLASS()
class PINBALLSURVIVAL_API ACombatantBaseClass : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatantBaseClass();

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
	
	UPROPERTY(VisibleAnywhere)
	UProjectileWeapons* ProjectileWeapons;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	UPROPERTY()
	TArray<AActor*> ProximityEnemyArray;
	UFUNCTION()
	TArray<AActor*> GetProximityEnemyArray() {return ProximityEnemyArray;}
};
