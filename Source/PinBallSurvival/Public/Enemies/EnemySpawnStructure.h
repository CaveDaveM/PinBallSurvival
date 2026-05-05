// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemySpawnStructure.generated.h"


//class AEnemyBase;

class AMyPawn;

USTRUCT()
struct FWaveSelection
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyPawn> Enemy;
	
	UPROPERTY(EditAnywhere)
	int16 Amount = 1;
	
	UPROPERTY(EditAnywhere)
	float SpawnDelay = 0.01f;
	
	UPROPERTY(EditAnywhere)
	float EndSectionInterval = 0.3;
};

USTRUCT()
struct FWaveSpawnParams : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY(EditAnywhere, Category = "WaveData")
	TArray<FWaveSelection> Sections;
	
	float EndWaveIntervals = 5.0f;
	
};