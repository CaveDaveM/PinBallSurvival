// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeClasses/PinballGameMode.h"

#include "GameModeClasses/PinballGameState.h"
#include "Kismet/GameplayStatics.h"

APinballGameMode::APinballGameMode()
{
}

void APinballGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(
		GameStart_TimeHandle,
		this,
		&APinballGameMode::GameStart,
		5.0f);
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player controller found"));
	}
	
	PC->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	PC->SetInputMode(InputMode);
}

void APinballGameMode::GameStart()
{
	if (APinballGameState* PinballGS = GetWorld()->GetGameState<APinballGameState>())
	{
		PinballGS->SetGamePhase(EGamePhase::StartPlay);
	}
}

void APinballGameMode::GameEnd()
{
}

void APinballGameMode::AddPoints()
{
}

void APinballGameMode::CheckPointsWin()
{
}
