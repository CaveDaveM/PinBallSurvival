// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProgressionSubsystem.h"

#include "GameFramework/SaveGame.h"
#include "Subsystems/SaveGameSubsytem.h"
#include "Subsystems/WorldStateSubsystem.h"

DEFINE_LOG_CATEGORY(LOGPlayerProgression);

void UPlayerProgressionSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	SaveGame = InWorld.GetGameInstance()->GetSubsystem<USaveGameSubsytem>();
	if (!SaveGame)
	{
		UE_LOG(LOGPlayerProgression, Warning, TEXT("Could not find SaveGameSubsystem in PlayerProgression"));
	}
	else
	{
		UE_LOG(LOGPlayerProgression, Warning, TEXT("Could find SaveGameSubsystem in PlayerProgression"));
	}
	
	WorldState = GetWorld()->GetSubsystem<UWorldStateSubsystem>();
	if (WorldState)
	{
		WorldState->OnGameEnded.AddUObject(this, &UPlayerProgressionSubsystem::SaveOnEndGame);
	}
}

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
	EnemiesKillCount++;
	CurrentXP += Amount;
	if (CurrentXP >= RequiredXP )
	{
		CurrentLevel++;
		CurrentXP = CurrentXP - RequiredXP;
		RequiredXP = CalculateNextLevelXPRequirement();
	}
	OnLevelUp.Broadcast(CurrentLevel);
}

int32 UPlayerProgressionSubsystem::CalculateEndGameXP(bool bIsGameWon)
{
	if (bIsGameWon)
	{
		return EnemiesKillCount;
	}
	else
	{
		return EnemiesKillCount / 2;
	}
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

void UPlayerProgressionSubsystem::SaveOnEndGame(EGamePhase GamePhase, bool bIsGameWon)
{
	if (GamePhase != EGamePhase::Ended)
	{
		UE_LOG(LOGPlayerProgression, Warning , TEXT("PlayerSubsystem, on game end reporting game not ended"))
		return; 
	}
	
	int32 EndGameXP = CalculateEndGameXP(bIsGameWon);
	SaveGame->SetPlayerXP(EndGameXP);
	SaveGame->SaveGame();

}
