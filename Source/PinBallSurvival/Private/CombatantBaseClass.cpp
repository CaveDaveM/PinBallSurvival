// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatantBaseClass.h"

#include "EnhancedInputComponent.h"
#include "EPinCollisionChannel.h"
#include "Components/SphereComponent.h"
#include "Projectile/BasicProjectile.h"

// Sets default values
ACombatantBaseClass::ACombatantBaseClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
// Called every frame
void ACombatantBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Called when the game starts or when spawned
void ACombatantBaseClass::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	
}
//Firing Weapons
void ACombatantBaseClass::OnClick()
{
	FHitResult Hit;
	
	if (PC->GetHitResultUnderCursor(ECC_Visibility,false, Hit))
	{
		FVector ClickLocation = Hit.Location;
		FireWeapon(ClickLocation);
	}
}

void ACombatantBaseClass::FireWeapon(FVector ClickedLocation)
{
	if (CurrentAmmo > 0)
	{

		
		
		FVector CurrentLocation = GetActorLocation();
		//Spawn Projectile
		FVector ProjectileDirection = ClickedLocation - CurrentLocation;
		ProjectileDirection.Z = 0.0f;
		FRotator ProjectileRotation = ProjectileDirection.Rotation();
		FTransform SpawnTransform = FTransform(ProjectileRotation, CurrentLocation);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABasicProjectile* Projectile = GetWorld()->SpawnActor<ABasicProjectile>(
			ProjectileWeapons,
			SpawnTransform,
			SpawnParams);
		if (Projectile)
		{
			Projectile->OverlapComponent->MoveIgnoreActors.Add(this);
			Projectile->ProjectileDamage = TotalDamage;
			CurrentAmmo--;
			ShotFired();
		}
	}
}
void ACombatantBaseClass::ShotFired()
{
}
void ACombatantBaseClass::UpdatePlayerHealth()
{
	//TODO: Cant remember what i was doing here
}
void ACombatantBaseClass::UpdateHudStats()
{
}
void ACombatantBaseClass::AddAmmo(const int32 Ammo)
{
	IWeaponInterface::AddAmmo(Ammo);
	
	const int32 PredictedAmmo = CurrentAmmo + Ammo;
	if (PredictedAmmo <= MaxAmmo)
	{
		CurrentAmmo = PredictedAmmo;
	}
	else
	{
		CurrentAmmo = MaxAmmo;
	}
	UpdateHudStats();
}
// Called to bind functionality to input
void ACombatantBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &ACombatantBaseClass::OnClick);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,TEXT("Move Input Component invalid"));
	}
}

