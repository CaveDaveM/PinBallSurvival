// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeClasses/PinballPlayerController.h"

void APinballPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}
