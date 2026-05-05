// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsytem.generated.h"

class UPinballSaveData;
DECLARE_LOG_CATEGORY_EXTERN(LOGSaveGameSystem, Display, All);
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API USaveGameSubsytem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	bool SaveGame();
	bool LoadGame();
	void SetPlayerXP(int32 NewPlayerXP);
	int32 GetPlayerXP();
protected:
	UPROPERTY()
	UPinballSaveData* CurrentSaveData;
	
	UPROPERTY()
	FString SaveName = TEXT("PlayerSave");
	
	UPROPERTY()
	int32 PlayerIndex = 0;
};
