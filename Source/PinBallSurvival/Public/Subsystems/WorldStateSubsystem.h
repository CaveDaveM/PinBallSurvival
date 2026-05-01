// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeClasses/PinballGameState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldStateSubsystem.generated.h"

enum class EObjectType : uint8;
class ABaseWorldObject;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStart, EGamePhase);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameInProgress, EGamePhase);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameEnded, EGamePhase);

UCLASS()
class PINBALLSURVIVAL_API UWorldStateSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void RegisterWorldObject(ABaseWorldObject* SpawnedObject);
	void UnregisterWorldObject(ABaseWorldObject* SpawnedObject);
	TArray<ABaseWorldObject*> GetRegisteredWorldObjects() const {return RegisteredHealingObjects;}
	
	ABaseWorldObject* GetClosestWorldObjectByType(EObjectType Type, FVector ActorLocation);
	ABaseWorldObject* FindClosestObjectFromLocation(TArray<ABaseWorldObject*> RegisteredObjects, const FVector& ActorLocation);
	
	FOnGameStart OnGameStart;
	FOnGameInProgress OnGameInProgress;
	FOnGameEnded OnGameEnded;
private:
	void SwitchOnGameStateChange(EGamePhase ChangedState);
	
	UPROPERTY()
	TArray<ABaseWorldObject*> RegisteredHealingObjects;
	
	UPROPERTY()
	TArray<ABaseWorldObject*> RegisteredAmmoObjects;
	
	UPROPERTY()
	TArray<ABaseWorldObject*> RegisteredUpgradeObjects;
};
