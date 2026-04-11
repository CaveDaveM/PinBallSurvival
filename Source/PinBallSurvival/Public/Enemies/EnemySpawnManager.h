// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawnStructure.h"
#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"
#include "EnemySpawnManager.generated.h"

class UBoxComponent;

USTRUCT()
struct FEnemySpawnExampleTable
{
	GENERATED_BODY()
	
	int32 Wave = 0;
	
	UPROPERTY()
	TArray<int32> EnemiesToSpawn;
};

UCLASS()
class PINBALLSURVIVAL_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()

	
public:	
	AEnemySpawnManager();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** At this stage i cant figure out how to get the extends of the box and transform them into coordinates.
	 * I will use a Hard Set approach for now **/
	
	TArray<FVector> FindSpawnOffsets(float radius);
	void DebugTestLocations();
	void StartWave();
	void StartWaveSelection();
	void SpawnEnemies();
	void EndWave();
	
	UPROPERTY()
	APawn* PlayerReference;
	
	UPROPERTY(EditInstanceOnly, Category = "SpawnBounds")
	FVector BoundsLocationX;
	
	UPROPERTY(EditInstanceOnly, Category = "SpawnBounds")
	FVector BoundsLocationY;
	
	UPROPERTY(EditAnywhere, Category = "Bounds")
	TObjectPtr<UBoxComponent> EnemySpawnBounds;
	
	UPROPERTY()
	FEnemySpawnExampleTable EnemySpawnExampleTable;
	UPROPERTY(EditDefaultsOnly, Category = "Wave Params")
	UDataTable* WaveSpawnTable;
	
	const FWaveSpawnParams* CurrentWaveData;
	
	FTimerHandle WaveSpawner_TimeHandler;
	
	int16 CurrentWaveInt = 0;
	int32 EnemyCount = 0;
	bool bSpawningComplete = false;
	FString RowName = "Wave_";
	TArray<FVector> EnemySpawnOffsets;
	
	FName GetCurrentWaveName() const {return FName(*(RowName + FString::FromInt(CurrentWaveInt)));}
};
