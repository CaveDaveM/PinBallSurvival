// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjectData.h"
#include "GameFramework/Actor.h"
#include "ObjectSpawnManager.generated.h"


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
	void SpawnWorldObjects();
	void SortWorldObjects();
	EObjectRarity FindRarity();
	
	FTimerHandle SpawnWorldObjects_TimerHandle;
	
	UPROPERTY()
	TArray<FWorldObjectData> CommonObjects;
	UPROPERTY()
	TArray<FWorldObjectData> RareObjects;
	UPROPERTY()
	TArray<FWorldObjectData> UniqueObjects;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
