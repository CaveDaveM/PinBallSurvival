// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveSpawnDataStructure.generated.h"

class AEnemyBaseClass;
/**
 * this will handle the data for the waves
 */
USTRUCT()
struct FWaveData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyBaseClass> Enemy;

	UPROPERTY(EditAnywhere)
	int16 Amount = 1;
	
};
