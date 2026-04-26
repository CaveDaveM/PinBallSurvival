// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/WeaponInterface.h"
#include "WorldObjects/BaseWorldObject.h"
#include "AmmoPack.generated.h"

/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API AAmmoPack : public ABaseWorldObject
{
	GENERATED_BODY()
	public:
	AAmmoPack();
	virtual void BeginPlay() override;

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void SetObjectRarity(EObjectRarity Rarity) override;
	
	int32 AmmoPackAmount = 40;
};
