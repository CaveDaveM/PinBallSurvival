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
	AHealthPack();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void SetObjectRarity(EObjectRarity Rarity) override;

	UPROPERTY(EditAnywhere)
	float HealAmount = 40;

};
