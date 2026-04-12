// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProgressionSubsystem.h"

void UPlayerProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PlayerStats.MaxHealth = 100.0f;
	PlayerStats.MoveSpeedScalar = 0.01f;
	PlayerStats.ProjectileDamage = 15.0f;
}

void UPlayerProgressionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}


void UPlayerProgressionSubsystem::AddXP(int32 Amount)
{
	CurrentXP += Amount;
	while (CurrentXP >= RequiredXP )
	{
		CurrentLevel++;
		CurrentXP = CurrentXP - RequiredXP;
		RequiredXP = CalculateNextLevelXPRequirement();
		OnLevelUp.Broadcast(CurrentLevel);
	}
}

int32 UPlayerProgressionSubsystem::CalculateNextLevelXPRequirement()
{
	// temp maths will be changed later 
	int32 Scalar = CurrentLevel <= 10? Scalar = 1: Scalar = 2;
	int32 NextLevelRequirement = CurrentLevel * Scalar + 5;
	return NextLevelRequirement;
}

FUpgradeData UPlayerProgressionSubsystem::GetUpgradeValues(FUpgradeData DecidedUpgrade)
{
	switch (DecidedUpgrade.UpgradeType) {
	case MoveSpeed:
		{
			DecidedUpgrade.CurrentLevel++;
			DecidedUpgrade.DisplayName = "Move Speed";
			DecidedUpgrade.UpgradeAmount += 15.0f;
			break;
		}
	case ProjectileDamage:
		{
			DecidedUpgrade.CurrentLevel++;
			DecidedUpgrade.DisplayName = "Projectile Damage";
			DecidedUpgrade.UpgradeAmount += 10.0f;
			break;
		}
	case MaxHealth:
		{
			DecidedUpgrade.CurrentLevel++;
			DecidedUpgrade.DisplayName = "Max Health";
			DecidedUpgrade.UpgradeAmount += 50.0f;
			break;
		}
	}
	return DecidedUpgrade;
}
