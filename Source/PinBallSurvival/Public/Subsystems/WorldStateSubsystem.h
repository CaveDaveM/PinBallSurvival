// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldObjects/BaseWorldObject.h"
#include "WorldStateSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API UWorldStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void RegisterWorldObject(ABaseWorldObject* SpawnedObject);
	void UnregisterWorldObject(ABaseWorldObject* SpawnedObject);
	TArray<ABaseWorldObject*> GetRegisteredWorldObjects() const {return RegisteredHealingObjects;}
	
	
private:
	UPROPERTY()
	TArray<ABaseWorldObject*> RegisteredHealingObjects;
	
	UPROPERTY()
	TArray<ABaseWorldObject*> RegisteredAmmoObjects;
	
	UPROPERTY()
	TArray<ABaseWorldObject*> RegisteredUpgradeObjects;
};
