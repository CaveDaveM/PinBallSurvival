// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "WhirlpoolActor.generated.h"

class APinballCharacter;
class UArrowComponent;
class USphereComponent;

//I Spelt it wrong, oops
UCLASS()
class PINBALLSURVIVAL_API AWhirlpoolActor : public AActor
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AWhirlpoolActor();
	
	UPROPERTY(EditAnywhere)
	USphereComponent* OverlapComponent;
	
	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowComponent;
	
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* WhirlpoolFX;
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UNiagaraComponent* SpawnedWhirlpool;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void TeleportPlayer(APinballCharacter* Player);
};
