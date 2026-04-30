// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballComponents/WhirlpoolActor.h"
#include "EPinCollisionChannel.h"
#include "NiagaraFunctionLibrary.h"
#include "PinballCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AWhirlpoolActor::AWhirlpoolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	OverlapComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(OverlapComponent);
	OverlapComponent->SetSphereRadius(500.0f);
	OverlapComponent->SetGenerateOverlapEvents(true);
	OverlapComponent->SetCollisionObjectType(ECC_WORLD_OBJECT);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_PLAYER,ECR_Overlap);
	
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	ArrowComponent->SetupAttachment(OverlapComponent);

}

void AWhirlpoolActor::BeginPlay()
{
	Super::BeginPlay();
	
	FVector SpawnLocation = GetActorLocation() + FVector(0.0f,0.0f,100.0f);
	SpawnedWhirlpool = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		WhirlpoolFX,
		SpawnLocation,
		FRotator::ZeroRotator);
	
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AWhirlpoolActor::OnOverlapBegin);
}

void AWhirlpoolActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APinballCharacter* PlayerCharacter = Cast<APinballCharacter>(OtherActor))
	{
		TeleportPlayer(PlayerCharacter);
	}
}

void AWhirlpoolActor::TeleportPlayer(APinballCharacter* Player)
{
	FVector NewLocation = ArrowComponent->GetComponentLocation();
	FRotator NewRotation = Player->GetActorRotation();
	Player->SetActorLocationAndRotation(
		NewLocation,
		NewRotation,
		false,
		nullptr,
		ETeleportType::TeleportPhysics);
	
}

void AWhirlpoolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
