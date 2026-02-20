// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyAIController.h"

#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(EnemyAILOG);

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	OwningPawn = InPawn;
	if (OwningPawn == nullptr)
	{
		UE_LOG(EnemyAILOG,Warning,TEXT(" Not Found Owning Pawn"))
	}
}


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacterReference = Cast<APawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (PlayerCharacterReference && OwningPawn && NavSystem)
	{
		GetWorld()->GetTimerManager().SetTimer(
			Action_TimeHandler,
			this,
			&AEnemyAIController::MoveEnemyToPlayer,
			0.5f,
			true);
	}
	
}
FVector AEnemyAIController::GetPlayerLocation()
{
	if (!PlayerCharacterReference)
	{
		UE_LOG(EnemyAILOG,Warning,TEXT(" Not Found Owning Pawn"))
		return FVector::ZeroVector;
	}
	
	FVector PlayerLocation = PlayerCharacterReference->GetActorLocation();
	return PlayerLocation;
}

void AEnemyAIController::MoveEnemyToPlayer()
{
	FVector PlayerLocation = GetPlayerLocation();
	
	MoveToLocation(PlayerLocation,10.0f,false,true,true,false,0,true);
}

