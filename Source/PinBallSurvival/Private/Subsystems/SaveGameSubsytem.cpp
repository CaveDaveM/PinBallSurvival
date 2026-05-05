// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SaveGameSubsytem.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystems/SaveData/PinballSaveData.h"
DEFINE_LOG_CATEGORY(LOGSaveGameSystem);
void USaveGameSubsytem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// this is not working i think
	UE_LOG(LOGSaveGameSystem, Log, TEXT("SaveGameSubsystem::Initialize START"));
	
	if (!LoadGame())
	{
		CurrentSaveData = Cast<UPinballSaveData>( 
			UGameplayStatics::CreateSaveGameObject(UPinballSaveData::StaticClass()));
		UE_LOG(LOGSaveGameSystem, Warning, TEXT("Created fresh save object: %s"),
			CurrentSaveData ? TEXT("OK") : TEXT("FAILED"));
	}
	UE_LOG(LOGSaveGameSystem, Warning, TEXT("SaveGameSubsystem::Initialize END"));
}

void USaveGameSubsytem::Deinitialize()
{
	Super::Deinitialize();
}

bool USaveGameSubsytem::SaveGame()
{
	
	UE_LOG(LOGSaveGameSystem, Warning, TEXT("SaveGame called, CurrentSaveData=%s"),
	CurrentSaveData ? TEXT("valid") : TEXT("NULL"));
	
	if (CurrentSaveData)
	{
		bool bResult = UGameplayStatics::SaveGameToSlot(CurrentSaveData, SaveName,PlayerIndex);
		UE_LOG(LOGSaveGameSystem, Warning, TEXT("SaveGameToSlot returned %s for slot '%s'"),
			 bResult ? TEXT("true") : TEXT("false"), *SaveName);
		
		return bResult;
	}
	else
	{
		return false;
	}
}

bool USaveGameSubsytem::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, PlayerIndex))
	{
		USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SaveName, PlayerIndex);
		CurrentSaveData = Cast<UPinballSaveData>(Loaded);
	}
	return CurrentSaveData != nullptr;	
}

void USaveGameSubsytem::SetPlayerXP(int32 NewPlayerXP)
{
	if (CurrentSaveData)
	{
		int32 OldPlayerXP = CurrentSaveData->PlayerXP;
		int32 PlayerXP = OldPlayerXP + NewPlayerXP;
		CurrentSaveData->PlayerXP = PlayerXP;
	}
}

int32 USaveGameSubsytem::GetPlayerXP()
{
	if (CurrentSaveData)
	{
		return CurrentSaveData->PlayerXP;
	}
	return 1000;
}
