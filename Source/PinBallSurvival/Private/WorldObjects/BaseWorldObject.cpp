// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/BaseWorldObject.h"

#include "EPinCollisionChannel.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"


// Sets default values
ABaseWorldObject::ABaseWorldObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthMesh = CreateDefaultSubobject<UStaticMeshComponent>("HealthMesh");
	SetRootComponent(HealthMesh);
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->SetupAttachment(HealthMesh);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionObjectType(ECC_INTERACTABLE);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_PLAYER,ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_ENEMY,ECR_Overlap);

}

// Called when the game starts or when spawned
void ABaseWorldObject::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseWorldObject::OnOverlapBegin);
	
	
}

void ABaseWorldObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,TEXT("Healing"));
}

void ABaseWorldObject::SpawnVisualEffects()
{
	SpawnIndication = UNiagaraFunctionLibrary::SpawnSystemAttached(
		SpawnIndicationClass,
		RootComponent,                    // attach to actor's root
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true); 
	if (SpawnIndication)
	{
		switch (ObjectType)
		{
		case EObjectType::HealingObject:
			{
				SpawnIndication->SetVariableLinearColor(
				TEXT("User.Color"),
				FLinearColor(0.0f,1.0f,0.0f,1.0f));
				break;
			}
		case EObjectType::AmmoObject:
			{
				SpawnIndication->SetVariableLinearColor(
				TEXT("User.Color"),
				FLinearColor(1.0f,0.0f,0.0f,1.0f));
				break;
			}
		case EObjectType::PlayerUpgrades:
			{
				SpawnIndication->SetVariableLinearColor(
				TEXT("User.Color"),
				FLinearColor(1.0f,1.0f,0.0f,1.0f));
				break;
			}
		default: ;
		}
	}
}


void ABaseWorldObject::SetObjectRarity(EObjectRarity Rarity)
{
}


// Called every frame
void ABaseWorldObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


