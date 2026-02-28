// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatantBaseClass.h"

#include "ProjectileWeapons.h"
#include "Components/SphereComponent.h"
#include "Interfaces/EnemyInterface.h"

// Sets default values
ACombatantBaseClass::ACombatantBaseClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PawnDetectionSphere = CreateDefaultSubobject<USphereComponent>("PawnDetectionSphere");
	PawnDetectionSphere->SetSphereRadius(600.0f);
	PawnDetectionSphere->SetGenerateOverlapEvents(true);
	PawnDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	/*PawnDetectionSphere->SetCollisionObjectType(EPinCollisionChannel::ECC_Player);
	PawnDetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PawnDetectionSphere->SetCollisionResponseToChannel(EPinCollisionChannel::ECC_Enemy, ECR_Overlap);*/
	//PawnDetectionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	ProjectileWeapons = CreateDefaultSubobject<UProjectileWeapons>("ProjectileWeaponsComponents");


}

// Called when the game starts or when spawned
void ACombatantBaseClass::BeginPlay()
{
	Super::BeginPlay();


	if (ProjectileWeapons != nullptr)
	{
		PawnDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACombatantBaseClass::OnOverlapBegin);
	}
	
}

void ACombatantBaseClass::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			ProximityEnemyArray.Add(OtherActor);
			ProjectileWeapons->SetProximityFromPawn();
		}
	}
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

