// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/RangedEnemy.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/EnemyProjectiles.h"

ARangedEnemy::ARangedEnemy()
{
	
}

void ARangedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacterReference = Cast<APawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacterReference)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ShootTimer_TimeHandler,
			this,
			&ARangedEnemy::ShootWeapon,
			5.0f,
			true);
	}
}

void ARangedEnemy::AddAmmo(const int32 Ammo)
{
	IWeaponInterface::AddAmmo(Ammo);
	int32 PredictedAmmo = CurrentAmmo + Ammo;
	if (PredictedAmmo >= MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
	}
	else
	{
		CurrentAmmo = PredictedAmmo;
	}
}

void ARangedEnemy::ShootWeapon()
{
	float Distance = GetDistanceTo(PlayerCharacterReference);
	if (Distance <= TargetDistance && CurrentAmmo != 0)
	{
		FVector TargetLocation = PlayerCharacterReference->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		//Spawn Projectile
		FRotator ProjectileRotation = (TargetLocation - CurrentLocation).Rotation();
		FTransform SpawnTransform = FTransform(ProjectileRotation, CurrentLocation);
			
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
			
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AEnemyProjectiles* Projectile = GetWorld()->SpawnActor<AEnemyProjectiles>(ProjectileClass,SpawnTransform,SpawnParams);
		if (Projectile)
		{
			Projectile->OverlapComponent->MoveIgnoreActors.Add(this);
			CurrentAmmo--;
		}
		
	}
}

