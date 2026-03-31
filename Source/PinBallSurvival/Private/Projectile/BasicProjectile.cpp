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
	PrimaryActorTick.bCanEverTick = true;
	//setting all the basics
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	SetRootComponent(ProjectileMesh);
	
	OverlapComponent = CreateDefaultSubobject<USphereComponent>("OverlapComponent");
	OverlapComponent->SetupAttachment(RootComponent);
	OverlapComponent->SetGenerateOverlapEvents(true);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionObjectType(EPinCollisionChannel::ECC_Bullet);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	OverlapComponent->SetCollisionResponseToChannel(EPinCollisionChannel::ECC_Enemy, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
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
		this, &ABasicProjectile::DespawnProjectile, DespawnDelay);
}

void ABasicProjectile::OnOverlaBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f,FColor::Cyan,TEXT("ENEMY HIT"));
	if (OtherActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
	{
		if (IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(OtherActor))
		{
			EnemyInterface->ApplyDamage(ProjectileDamage);
		}
	}
}



// Called every frame
void ABasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicProjectile::DespawnProjectile()
{
	Destroy();
}

