// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HealthInterface.h"
#include "BasicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class PINBALLSURVIVAL_API ABasicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicProjectile();
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* OverlapComponent;
	
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnOverlaBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void DespawnProjectile();
	void CheckPiercingPower();
	
	UPROPERTY(EditDefaultsOnly)
	float ProjectileDamage = 40.0f;

	UPROPERTY(EditDefaultsOnly)
	float DespawnDelay = 10.0f;
	
	FTimerHandle Despawn_TimerHandle;
	
	UPROPERTY()
	int32 NumberOfActorsHit = 0;
	
	UPROPERTY()
	int32 PiercingPower = 16;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE void SetProjectileDamage(const float SetDamage){ProjectileDamage = SetDamage;}

};
