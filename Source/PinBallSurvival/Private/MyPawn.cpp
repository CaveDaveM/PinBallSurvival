// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

#include "PinBallCollisionChannels.h"
#include "Components/SphereComponent.h"
#include "Enemies/EnemyAIController.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AMyPawn::AMyPawn()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	
	PawnMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(PawnMeshComponent);
	
	PawnOverlapComponent = CreateDefaultSubobject<USphereComponent>("OverlapComponent");
	PawnOverlapComponent->SetupAttachment(RootComponent);
	PawnOverlapComponent->SetSphereRadius(25);
	PawnOverlapComponent->SetGenerateOverlapEvents(true);
	//PawnOverlapComponent->SetCollisionProfileName(TEXT("EnemyCollision"));
	PawnOverlapComponent->SetCollisionObjectType(EPinBallCollisionChannel::ECC_Enemy);
	PawnOverlapComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

