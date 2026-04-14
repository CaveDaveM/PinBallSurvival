// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProgressionSubsystem.h"

void UPlayerProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPlayerProgressionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}


void UPlayerProgressionSubsystem::AddXP(int32 Amount)
{
	CurrentXP += Amount;
	if (CurrentXP >= RequiredXP )
	{
		CurrentLevel++;
		CurrentXP = CurrentXP - RequiredXP;
		RequiredXP = CalculateNextLevelXPRequirement();
	}
	OnLevelUp.Broadcast(CurrentLevel);
}

int32 UPlayerProgressionSubsystem::CalculateNextLevelXPRequirement()
{
	// temp maths will be changed later 
	int32 Scalar = CurrentLevel <= 10? Scalar = 1: Scalar = 2;
	int32 NextLevelRequirement = CurrentLevel * Scalar + 5;
	return NextLevelRequirement;
}

void UPlayerProgressionSubsystem::SetUpgradeValues(EUpgrades DecidedUpgrade)
{
	switch (DecidedUpgrade) {
	case MoveSpeed:
		{
			PlayerStats.MoveSpeedScalar += 0.01f;
			break;
		}
	case ProjectileDamage:
		{
			PlayerStats.ProjectileDamage += 15.0f;
			break;
		}
	case MaxHealth:
		{
			PlayerStats.MaxHealth += 50.0f;
			break;
		}
	}
	OnPlayerStats.Broadcast(PlayerStats);
}
