// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerProgressionSubsystem.generated.h"

/**
 * 
 */
//Broadcasts when the player levels up.
// struct and enum to store and move the data for each upgrade type,
UENUM()
enum EUpgrades : uint8
{
	MoveSpeed UMETA(DisplayName = "MoveSpeed"),
	ProjectileDamage UMETA(DisplayName = "ProjectileDamage"),
	MaxHealth UMETA(DisplayName = "MaxHealth"),
};

USTRUCT()
struct FUpgradeData
{
	GENERATED_BODY()

	EUpgrades UpgradeType;
	
	UPROPERTY(EditAnywhere)
	FString DisplayName;
	
	UPROPERTY(EditAnywhere)
	int32 CurrentLevel = 1;
	
	UPROPERTY(EditAnywhere)
	int32 UpgradeAmount = 0;
};

USTRUCT()
struct FPlayerStats
{
	GENERATED_BODY()
	
	float MoveSpeedScalar = 1.01f;
	float ProjectileDamage = 1.0f;
	float MaxHealth = 100.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStats, FPlayerStats, PlayerStats);

UCLASS()
class PINBALLSURVIVAL_API UPlayerProgressionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UPROPERTY()
	FOnLevelUp OnLevelUp;
	FPlayerStats PlayerStats;
	FOnPlayerStats OnPlayerStats;
	
	void AddXP(int32 Amount);
	int32 GetCurrentLevel() const { return CurrentLevel; }
	int32 GetCurrentXP() const { return CurrentXP; }
	int32 CalculateXPForNextLevel() const {return RequiredXP - CurrentXP;}
	int32 CalculateNextLevelXPRequirement();
	FPlayerStats GetPlayerStats() const { return PlayerStats; }
	
	void SetUpgradeValues(EUpgrades DecidedUpgrade);

private:
	int32 CurrentLevel = 1;
	int32 CurrentXP = 0;
	int32 RequiredXP = 5;
	int32 StoredLevels = 0;
};
