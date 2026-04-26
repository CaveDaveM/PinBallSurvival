// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PinballGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API APinballGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
    APinballGameMode();
	virtual void BeginPlay() override;
    
	void GameEnd();
	void AddPoints();
	
private:
	void GameStart();
	void CheckPointsWin();
	
	int32 WinScore = 10000;
	FTimerHandle GameStart_TimeHandle;
};
