// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PinballSaveData.generated.h"

/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API UPinballSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	int32 PlayerXP = 0;
	
};
