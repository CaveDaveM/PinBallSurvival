// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/HealthInterface.h"
#include "MyPawn.generated.h"

class USphereComponent;
class UFloatingPawnMovement;

UCLASS()
class PINBALLSURVIVAL_API AMyPawn : public APawn, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

		
	UPROPERTY(editanywhere)
	UStaticMeshComponent* PawnMeshComponent;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* PawnOverlapComponent;
	
	UPROPERTY(EditDefaultsOnly)
	float Health = 100;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void CheckHealth();

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ApplyDamage(float DamageAmount) override;
};
