// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/EnemyInterface.h"
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
	float ProjectileDamage;
	
	UPROPERTY()
	USphereComponent* OverlapComponent;
	
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY()
	float Damage = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE void SetProjectileDamage(const float SetDamage){Damage = SetDamage;}

};
