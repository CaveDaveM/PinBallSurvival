// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/HealthPack.h"

#include "PinballCharacter.h"
#include "PinBallCollisionChannels.h"
#include "Components/SphereComponent.h"

// Make the health packs random on spawn;
AHealthPack::AHealthPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

// Called when the game starts or when spawned
void AHealthPack::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthPack::OnOverlapBegin);
	
	
}



// Called every frame
void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,TEXT("Healing"));
	if (APinballCharacter* PlayerCharacter = Cast<APinballCharacter>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,TEXT("Healing"));
		PlayerCharacter->ApplyHealing(HealAmount);
		Destroy();
	}
}

