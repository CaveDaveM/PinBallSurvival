// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeClasses/PinballGameState.h"

void APinballGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (GamePhase == Playing)
	{
		ElapsedTime += DeltaSeconds;
	}
}

void APinballGameState::SetCurrentScore(int32 NewScore)
{
	CurrentScore = NewScore;
	OnScoreChange.Broadcast(CurrentScore);
}

void APinballGameState::SetGamePhase(EGamePhase NewPhase)
{
	// Need to set the game state for the game instance subsystem. since they are persistent from level to level. 
	GamePhase = NewPhase;
	OnStateChange.Broadcast(GamePhase);
}

void APinballGameState::SetHighScore()
{
	HighScore = CurrentScore;
}
