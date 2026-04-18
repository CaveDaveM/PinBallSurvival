// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatantBaseClass.h"

#include "EPinCollisionChannel.h"
#include "PinBallCollisionChannels.h"
#include "Components/SphereComponent.h"
#include "Projectile/BasicProjectile.h"

// Sets default values
ACombatantBaseClass::ACombatantBaseClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PawnDetectionSphere = CreateDefaultSubobject<USphereComponent>("PawnDetectionSphere");
	PawnDetectionSphere->SetSphereRadius(600.0f);
	PawnDetectionSphere->SetGenerateOverlapEvents(true);
	PawnDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PawnDetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PawnDetectionSphere->SetCollisionResponseToChannel(ECC_ENEMY,ECR_Overlap);

}

// Called every frame
void ACombatantBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACombatantBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called when the game starts or when spawned
void ACombatantBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
	PawnDetectionSphere->OnComponentBeginOverlap.AddDynamic(this,&ACombatantBaseClass::OnOverlapBegin);
	PawnDetectionSphere->OnComponentEndOverlap.AddDynamic(this,&ACombatantBaseClass::OverlapEnd);
	
	GetWorld()->GetTimerManager().SetTimer(
		FireWeapon_TimerHandle,
		this,
		&ACombatantBaseClass::FireWeapon,
		1.0f,
		true);
}

void ACombatantBaseClass::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
		{
			ProximityEnemyArray.Add(OtherActor);
#if 0			
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,  
			                                 FString::Printf(TEXT("Number Of Enemies: %d"), ProximityEnemyArray.Num()));
#endif
		}
	}
}

void ACombatantBaseClass::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
		{
			ProximityEnemyArray.Remove(OtherActor);
		}
	}
}

void ACombatantBaseClass::FireWeapon()
{
	if (ProximityEnemyArray.Num() > 0 && CurrentAmmo > 0)
	{

		AActor* CurrentTarget = ProximityEnemyArray[0];
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		//Spawn Projectile
		FRotator ProjectileRotation = (TargetLocation - CurrentLocation).Rotation();
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
}

