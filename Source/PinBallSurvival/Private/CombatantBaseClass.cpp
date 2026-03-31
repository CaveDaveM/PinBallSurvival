// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatantBaseClass.h"

#include "PinBallCollisionChannels.h"
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
	PawnDetectionSphere->SetCollisionResponseToChannel(EPinBallCollisionChannel::ECC_Enemy,ECR_Overlap);

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
		if (OtherActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			ProximityEnemyArray.Add(OtherActor);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,  
			FString::Printf(TEXT("Number Of Enemies: %d"), ProximityEnemyArray.Num()));
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
	if (ProximityEnemyArray.Num() > 0)
	{

		AActor* CurrentTarget = ProximityEnemyArray[0];
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		//Spawn Projectile
		FRotator ProjectileRotation = (TargetLocation - CurrentLocation).Rotation();
		FTransform SpawnTransform = FTransform(ProjectileRotation, CurrentLocation);
		
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

