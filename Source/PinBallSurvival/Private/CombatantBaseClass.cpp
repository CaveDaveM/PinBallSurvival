// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatantBaseClass.h"

#include "Components/SphereComponent.h"
#include "Interfaces/EnemyInterface.h"
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
	PawnDetectionSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

}

// Called when the game starts or when spawned
void ACombatantBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
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
	GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Yellow,TEXT("Overlap"));
	if (OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			ProximityEnemyArray.Add(OtherActor);
		}
	}
}

void ACombatantBaseClass::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Yellow,TEXT("EndOverlap"));

			ProximityEnemyArray.Remove(OtherActor);
		}
	}
}

// Called every frame
void ACombatantBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatantBaseClass::FireWeapon()
{
	
	//NOTE: THIS IS ALL TEMP
	GEngine->AddOnScreenDebugMessage(-1,2.0f, FColor::Green,"FireWeapon");

	if (ProximityEnemyArray.Num() > 1)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f, FColor::Green,"ArrayMoreThan1");

		AActor* CurrentTarget = ProximityEnemyArray[0];
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		//Spawn Projectile
		FRotator ProjectileRotation = (TargetLocation - CurrentLocation).Rotation();
		FTransform SpawnTransform = FTransform(ProjectileRotation, GetOwner()->GetActorLocation());
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABasicProjectile>(ProjectileWeapons,SpawnTransform,SpawnParams);
	}
}


// Called to bind functionality to input
void ACombatantBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

