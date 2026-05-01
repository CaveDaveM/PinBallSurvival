// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/EnemyProjectiles.h"

#include "EPinCollisionChannel.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HealthInterface.h"

// Sets default values
AEnemyProjectiles::AEnemyProjectiles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	SetRootComponent(ProjectileMesh);
	
	OverlapComponent = CreateDefaultSubobject<USphereComponent>("OverlapComponent");
	OverlapComponent->SetupAttachment(RootComponent);
	OverlapComponent->SetGenerateOverlapEvents(true);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionObjectType(ECC_BULLET);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_PLAYER, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 200.0f;
	ProjectileMovement->MaxSpeed = 200.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AEnemyProjectiles::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
		Despawn_TimeHandle,
		this,
		&AEnemyProjectiles::DestroyActor,
		DespawnDelay,
		false);
}


void AEnemyProjectiles::OnOverlaBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		if (IHealthInterface* EnemyInterface = Cast<IHealthInterface>(OtherActor))
		{
			EnemyInterface->ApplyDamage(ProjectileDamage);
			Destroy();
		}
	}
}
void AEnemyProjectiles::DestroyActor()
{
	Destroy();
}

// Called every frame
void AEnemyProjectiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

