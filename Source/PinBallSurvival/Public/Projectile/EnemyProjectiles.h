// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyProjectiles.generated.h"

class USphereComponent;

UCLASS()
class PINBALLSURVIVAL_API AEnemyProjectiles : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectiles();
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* OverlapComponent;
	
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void DestroyActor();
	void OnOverlaBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	float ProjectileDamage = 40.0f;
	float DespawnDelay = 5.0f;
	FTimerHandle Despawn_TimeHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
