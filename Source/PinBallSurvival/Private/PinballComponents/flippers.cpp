// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballComponents/flippers.h"
#include "PinballCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(GameInfo);

// Sets default values
Aflippers::Aflippers()
{
 	// 
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
	
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("DirectionOfForce");
	ArrowComponent->SetupAttachment(StaticMesh);
	/** the box component moves with the static mesh, but i dont know if i like this approach as the
	static mesh will be purely cosmetic **/
	BoxOverlapComp = CreateDefaultSubobject<UBoxComponent>("BoxOverlapComp");
	BoxOverlapComp->SetupAttachment(StaticMesh);
	BoxOverlapComp->SetGenerateOverlapEvents(true);
	BoxOverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxOverlapComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	
}


// Called when the game starts or when spawned
void Aflippers::BeginPlay()
{
	Super::BeginPlay();
	
	BoxOverlapComp->OnComponentBeginOverlap.AddDynamic(this, &Aflippers::OnBeginOverlap);
	
	/** the check cast here assumes that the cast will not fail. bad practice as the impossible is never impossible
		when code is concerned. reference "the pragmatic programming". **/ 
	PlayerCharacter = CastChecked<APinballCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
	{
		UE_LOG(GameInfo,Display,TEXT("FLIPPERS/Player character found and set"));
	}
	else
	{
		UE_LOG(GameInfo,Error,TEXT("FLIPPERS/Player character NotFound"));
	}
	
}

void Aflippers::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerCharacter)
	{
		ApplyForceToPlayer();
	}
}

void Aflippers::ApplyForceToPlayer()
{
	FVector RotationOfArrow = ArrowComponent->GetForwardVector();
	FVector RotationAndMagnitude = RotationOfArrow * PushStrength;
	UE_LOG(GameInfo, Warning, TEXT("RotationOfArrow Vector = %s"), *RotationAndMagnitude.ToString());
	PlayerCharacter->PlayerMesh->AddImpulse(RotationAndMagnitude,NAME_None,true);
}

// Called every frame
void Aflippers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

