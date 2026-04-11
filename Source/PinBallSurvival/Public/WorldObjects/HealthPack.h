// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWorldObject.h"
#include "GameFramework/Actor.h"
#include "HealthPack.generated.h"

class USphereComponent;

UCLASS()
class PINBALLSURVIVAL_API AHealthPack : public ABaseWorldObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPack();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	UPROPERTY(EditAnywhere)
	float HealAmount = 40;
	
	UPROPERTY(EditAnywhere)
	int8 HealingLevel;

};
