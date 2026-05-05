// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PinballGameState.generated.h"

/**
 * 
 */
UENUM()
enum EGamePhase : uint8
{
	None UMETA(Hidden),
	StartPlay UMETA(DisplayName = "StartPlay"),
	Playing UMETA(DisplayName = "Playing"),
	Ended UMETA(DisplayName = "Ended"),
};

// i read that dynamic delegates are something like 10x slower and im not going to be using these in 
// blueprints anyway so i dont need them to be dynamic
DECLARE_MULTICAST_DELEGATE_OneParam(FOnScoreChange, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStateChange, EGamePhase, bool);
UCLASS()
class PINBALLSURVIVAL_API APinballGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
	void SetCurrentScore(int32 NewScore);
	void SetGamePhase(EGamePhase NewPhase, bool bIsGameWon = false);
	void SetHighScore();
	
	FOnScoreChange OnScoreChange;
	FOnStateChange OnStateChange;

private:
	int32 CurrentScore = 0;
	int32 HighScore = 0;
	float ElapsedTime = 0.0f;
	
	EGamePhase GamePhase = None;
	
};
