// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileWeapons.generated.h"


class ACombatantBaseClass;
class ABasicProjectile;
class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PINBALLSURVIVAL_API UProjectileWeapons : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileWeapons();

	UFUNCTION()
	void SetProximityFromPawn();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void FireWeapon();
	
	UPROPERTY()
	TArray<AActor*> ProximityEnemyArray;
	
	FTimerHandle FireWeapon_TimerHandle;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//TEMP STUFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABasicProjectile> ProjectileWeapons;
	
	UPROPERTY()
	ACombatantBaseClass* OwningPawn;
};
