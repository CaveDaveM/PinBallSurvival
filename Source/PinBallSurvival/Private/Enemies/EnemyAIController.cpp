// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyAIController.h"

#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
}


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacterReference = Cast<APawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacterReference)
	{
		GetWorld()->GetTimerManager().SetTimer(
			Action_TimeHandler,
			this,
			&AEnemyAIController::MoveEnemyToPlayer,
			0.5f,
			true);
	}
}

void AEnemyAIController::MoveEnemyToPlayer()
{
}

FVector AEnemyAIController::FindPlayerLocation()
{
	return FVector::ZeroVector;
}

