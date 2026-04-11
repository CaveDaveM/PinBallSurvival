// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Managers/ObjectSpawnManager.h"

// Sets default values
AObjectSpawnManager::AObjectSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	SortWorldObjects();
	
	GetWorld()->GetTimerManager().SetTimer(
		SpawnWorldObjects_TimerHandle,
		this,
		&AObjectSpawnManager::SpawnWorldObjects,
		10.0f,
		true);
}

void AObjectSpawnManager::SortWorldObjects()
{
	for (FWorldObjectData WorldObject : WorldObjectData)
	{
		switch (WorldObject.Rarity) 
		{
			case EObjectRarity::Common:
				{
					CommonObjects.Add(WorldObject);
					break;
				}
			case EObjectRarity::Rare:
				{
					RareObjects.Add(WorldObject);
					break;
				}
			case EObjectRarity::Unique:
				{
					UniqueObjects.Add(WorldObject);
					break;
				}
			default:
				{
					UE_LOG(LogTemp, Warning, TEXT("Missing Rarity in World Object Data"));
				}
		}
	}
}

EObjectRarity AObjectSpawnManager::FindRarity()
{
}

void AObjectSpawnManager::SpawnWorldObjects()
{
}


// Called every frame
void AObjectSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

