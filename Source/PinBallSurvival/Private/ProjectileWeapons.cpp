// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapons.h"

#include "CombatantBaseClass.h"
#include "Projectile/BasicProjectile.h"

// Sets default values for this component's properties
UProjectileWeapons::UProjectileWeapons()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	// ...
}



void UProjectileWeapons::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPawn = Cast<ACombatantBaseClass>(GetOwner());
	
	GetWorld()->GetTimerManager().SetTimer(
		FireWeapon_TimerHandle,
		this,
		&UProjectileWeapons::FireWeapon,
		1.0f,
		true);
}

void UProjectileWeapons::SetProximityFromPawn()
{
	ProximityEnemyArray = OwningPawn->ProximityEnemyArray;
	
}

void UProjectileWeapons::FireWeapon()
{
	
	//NOTE: THIS IS ALL TEMP
	GEngine->AddOnScreenDebugMessage(-1,2.0f, FColor::Green,"FireWeapon");

	if (ProximityEnemyArray.Num() > 1)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f, FColor::Green,"ArrayMoreThan1");

		AActor* CurrentTarget = ProximityEnemyArray[0];
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		//Spawn Projectile
		FRotator ProjectileRotation = (TargetLocation - CurrentLocation).Rotation();
		FTransform SpawnTransform = FTransform(ProjectileRotation, GetOwner()->GetActorLocation());
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABasicProjectile>(ProjectileWeapons,SpawnTransform,SpawnParams);
	}
}

// Called every frame
void UProjectileWeapons::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

