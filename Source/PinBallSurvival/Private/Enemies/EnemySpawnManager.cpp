// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemySpawnManager.h"

#include "Components/BoxComponent.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EnemySpawnBounds = CreateDefaultSubobject<UBoxComponent>("EnemySpawnBounds");
	EnemySpawnBounds->SetBoundsScale(40.0f);
}

// Called when the game starts or when spawned
void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	const FVector Extent = EnemySpawnBounds->GetScaledBoxExtent();

	// Box transform
	const FTransform& Transform = EnemySpawnBounds->GetComponentTransform();

	// Local-space corners (bottom face, Z = -Extent.Z)
	TArray<FVector> LocalCorners =
	{
		FVector( Extent.X,  Extent.Y, -Extent.Z),
		FVector( Extent.X, -Extent.Y, -Extent.Z),
		FVector(-Extent.X, -Extent.Y, -Extent.Z),
		FVector(-Extent.X,  Extent.Y, -Extent.Z)
	};

	// Convert to world space
	TArray<FVector> WorldCorners;
	for (const FVector& Corner : LocalCorners)
	{
		WorldCorners.Add(Transform.TransformPosition(Corner));
	}
	
	
}

// Called every frame
void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

