// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BasicEnemy.h"

#include "PinballCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABasicEnemy::ABasicEnemy()
{
}

void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<APinballCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerCharacter)
	{
		GetWorld()->GetTimerManager().SetTimer(
			HitPlayer_TimerHandle,
			this,
			&ABasicEnemy::ApplyDamageIfInRange,
			0.5f,
			true);
	}
	
}

void ABasicEnemy::ApplyDamageIfInRange()
{
	 if (GetDistanceTo(PlayerCharacter) < 50.0f)
	 {
		 PlayerCharacter->ApplyDamage(EnemyDamage);
	 }
}
