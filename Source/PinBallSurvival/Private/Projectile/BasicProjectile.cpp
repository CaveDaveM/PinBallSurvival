// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/BasicProjectile.h"

#include "EPinCollisionChannel.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/EnemyInterface.h"

// Sets default values
ABasicProjectile::ABasicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//setting all the basics
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	SetRootComponent(ProjectileMesh);
	
	OverlapComponent = CreateDefaultSubobject<USphereComponent>("OverlapComponent");
	OverlapComponent->SetupAttachment(RootComponent);
	OverlapComponent->SetGenerateOverlapEvents(true);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionObjectType(ECC_BULLET);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_ENEMY, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}

// Called when the game starts or when spawned
void ABasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this,&ABasicProjectile::OnOverlaBegin);
	
	GetWorld()->GetTimerManager().SetTimer(
		Despawn_TimerHandle,
		this, &ABasicProjectile::DespawnProjectile,
		DespawnDelay);
	
	
}

void ABasicProjectile::OnOverlaBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		if (IHealthInterface* EnemyInterface = Cast<IHealthInterface>(OtherActor))
		{
			EnemyInterface->ApplyDamage(ProjectileDamage);
			NumberOfActorsHit++;
			CheckPiercingPower();
		}
	}
}



// Called every frame
void ABasicProjectile::CheckPiercingPower()
{
	if (NumberOfActorsHit == PiercingPower)
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,"Checking piercing power");
		Destroy();
	}
}

void ABasicProjectile::DespawnProjectile()
{
	Destroy();
}

void ABasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


