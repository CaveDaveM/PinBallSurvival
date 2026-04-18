// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjectData.h"
#include "GameFramework/Actor.h"
#include "Subsystems/WorldStateSubsystem.h"
#include "ObjectSpawnManager.generated.h"

USTRUCT()
struct FRarityWeights
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EObjectRarity Rarity;
	
	UPROPERTY(EditAnywhere)
	float Weight;
};

UCLASS()
class PINBALLSURVIVAL_API AObjectSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectSpawnManager();
	
	UPROPERTY(EditAnywhere)
	TArray<FWorldObjectData> WorldObjectData;
	

protected:
	// All Logic to make the weight based world object spawner
	virtual void BeginPlay() override;
	FVector3d FindSpawnLocation();
	EObjectRarity FindRarity();
	void FindWorldObjectsArray();
	void SortWorldObjects();
	void SpawnWorldObjects(const TArray<FWorldObjectData>& WorldObjects);
	UFUNCTION()
	void OnObjectCollected(AActor* CollectedObjects);
	
	UPROPERTY()
	UWorldStateSubsystem* WorldState;
	FTimerHandle SpawnWorldObjects_TimerHandle;
	
	UPROPERTY()
	TArray<FWorldObjectData> CommonObjects;
	UPROPERTY()
	TArray<FWorldObjectData> RareObjects;
	UPROPERTY()
	TArray<FWorldObjectData> UniqueObjects;
	
	//RarityFloatWeights
	TArray<FRarityWeights> RarityTable = {
		{EObjectRarity::Common,60.0f},
		{EObjectRarity::Rare,30.0f},
		{EObjectRarity::Unique,10.0f},
	};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//RarityFloatWeights
	void SetNewRarityTable(const TArray<FRarityWeights>& NewRarityTable);
	
};
