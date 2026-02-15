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
	
	/** NOTE: i do not like this implementation, its static and doesnt allow for a change of spawner shape.
	 * TODO: Find a way to make this a dynamic spawn location **/
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector SpawnLocation = FVector(
		FMath::RandRange(-9210.0f, 8710.0f),
		FMath::RandRange(-6970.0f, 6290.0f),
		50.0f);

	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	DrawDebugSphere(GetWorld(), SpawnLocation, 50.0f, 12, FColor::Red, true, 250.0f);

	/*AEnemyBase* Enemy = GetWorld()->SpawnActor<AEnemyBase>(
		CurrentWaveData->Sections[SectionInt].Enemy,
		SpawnLocation,
		SpawnRotation,
		SpawnParams);*/
	
	
}

// Called every frame
void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

