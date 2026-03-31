// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemySpawnManager.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	/** NOTE: i do not like this implementation, its static and doesnt allow for a change of spawner shape.
	 * TODO: Find a way to make this a dynamic spawn location **/
	
	PlayerReference = UGameplayStatics::GetPlayerPawn(this, 0);
	
	
	//TEST STRUCT
	TArray<int32> TestEnemyArray;
	for (int32 i = 0; i < 16; ++i)
	{
		TestEnemyArray.Add(i);
	}
	EnemySpawnExampleTable.EnemiesToSpawn = TestEnemyArray;
	
	DebugTestLocations();
}

TArray<FVector> AEnemySpawnManager::FindSpawnOffsets(float radius)
{
	const float AngleStep = 360.0f / EnemySpawnExampleTable.EnemiesToSpawn.Num();
	float CurrentAngle = 0.0f;
	TArray<FVector> EnemiesSpawnOffset;
	for (int i = 0; i < EnemySpawnExampleTable.EnemiesToSpawn.Num(); ++i)
	{
		float Radians = FMath::DegreesToRadians(CurrentAngle);
		FVector SpawnOffset(
			FMath::Cos(Radians) * radius,
			FMath::Sin(Radians) * radius,
			0.0f);
		
		EnemiesSpawnOffset.Add(SpawnOffset);
		CurrentAngle += AngleStep;
	}
	
	return EnemiesSpawnOffset;
}

void AEnemySpawnManager::DebugTestLocations()
{
	FVector PlayerLocation = PlayerReference->GetActorLocation();
	TArray<FVector> EnemiesSpawnOffset = FindSpawnOffsets(500.0f);
	for (int i = 0; i < EnemiesSpawnOffset.Num(); ++i)
	{
		FVector EnemySpawnLocation = PlayerLocation + EnemiesSpawnOffset[i];
		DrawDebugSphere(GetWorld(),EnemySpawnLocation,25.0f,30,FColor::Red,true);
	}
}

// Called every frame
void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

